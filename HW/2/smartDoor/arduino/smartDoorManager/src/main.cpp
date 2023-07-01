#include <Arduino.h>
#include <string.h>
#include <Keypad.h>


//time that show how many seconds  remaine  to closing the door
unsigned long int doors_limit_time=1000;
unsigned long int door1_timer=0;
unsigned long int door2_timer=0;
int door_num_input=0;
bool door1_status=false;
bool door2_status=false;
const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[rows] = {2,3,4,5}; //connect to the row pinouts of the keypad
byte colPins[cols] = {6,7,8}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

void setup() {
  Serial.begin(9600);


  
  // put your setup code here, to run once:

}



void openDoor(int doorID){
  String message;
  if (doorID==1){message="11";door1_status=true; door1_timer=millis();}// the first num shows openning door and the others  is door id.
  if (doorID==2){message="12";door2_status=true; door2_timer=millis();}
  Serial.println(message);}
void closeDoor(int doorID){
   String message;
  if (doorID==1){message="01";door1_status=false;}// the first num shows clossing door and the others  is door id.
  if (doorID==2){message="02";door2_status=false;}// the second num shows
  Serial.println(message);}

void DoorsTimer(){

  if (door1_status &&(millis() - door1_timer  > doors_limit_time)){closeDoor(1);}
  if (door2_status &&(millis() -  door2_timer > doors_limit_time)){closeDoor(2);}
}


void loop() {

  
  char key = keypad.getKey();
  if(key != NO_KEY){

   if (key!= '*' && key != '#'){door_num_input= 10 * door_num_input + int(key) - 48 ;}
   else if (key == '*'){ openDoor(door_num_input);door_num_input=0;}
   else{closeDoor(door_num_input);door_num_input=0;} }

  DoorsTimer();

}



