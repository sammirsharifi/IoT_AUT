#include <Arduino.h>
#include <string.h>
#include <Servo.h>

Servo myservo1;
Servo myservo2;

void setup() {
  Serial.begin(9600);
  myservo1.attach(3);
  myservo2.attach(5);
  myservo1.write(-45);
  myservo2.write(-45);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(10,HIGH);
  digitalWrite(12,HIGH);



}


void closeDoor(char doorID){
  if (doorID =='1'){digitalWrite(11,LOW);digitalWrite(12, HIGH);myservo1.write(-180);}
  if (doorID =='2'){digitalWrite(9,LOW);digitalWrite(10, HIGH);myservo2.write(-180);}
}
void openDoor(char doorID){
  if (doorID =='1'){digitalWrite(12,LOW);digitalWrite(11,HIGH);myservo1.write(180);}
  if (doorID =='2'){digitalWrite(10,LOW);digitalWrite(9,HIGH);myservo2.write(180);}
}

void loop() {
  if (Serial.available()){
    String data= Serial.readStringUntil('\n');
    if ( data[0] =='0' ){ closeDoor(data[1]);}
    else if ( data[0] =='1' ){ openDoor(data[1]);}

   

  }

}

