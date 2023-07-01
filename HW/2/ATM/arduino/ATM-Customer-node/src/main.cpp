#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

const int rs = 27, en = 26, d4 = 25, d5 = 24, d6 = 23, d7 = 22;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[rows] = {31,30,29,28}; //connect to the row pinouts of the keypad
byte colPins[cols] = {34,33,32}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

void menu(String id);

void menu_screen(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1.wallet");lcd.setCursor(0,1);
  lcd.print("2.transter");lcd.setCursor(0,2);
  lcd.print("3.history");lcd.setCursor(0,3);
  lcd.print("4.exit");
 } 
 bool login(String id , String pass){
    Serial.println("login");
    Serial.println(id);
    Serial.println(pass);
    String response;
    while (true)
    {
      if(Serial.available()){
       response= Serial.readStringUntil('\n');
      if (response[0]=='1'){return  true;}
      else{return false;}
    }
    
  }}
 void login_screen(){
  lcd.clear();
  lcd.print("Enter your id:");
  lcd.setCursor(0,1);
  lcd.print("then press #");
  lcd.setCursor(0,2);
  String  id="";
  char key=keypad.getKey();
  int col_index=0;
  while (true){
    if (key!=NO_KEY){ if (key=='#'){break;}
    lcd.setCursor(col_index,2);lcd.print(key);col_index++;id=id+key;}
    key=keypad.getKey();}
  lcd.clear();
  lcd.print("Enter your pass");
  lcd.setCursor(0,1);
  lcd.print("then press #");
  lcd.setCursor(0,2);
  String  pass="";
  col_index=0;
  key = keypad.getKey();
  while (true){
    if (key!=NO_KEY){ if (key=='#'){break;}
    lcd.setCursor(col_index,2);lcd.print(key);col_index++;pass=pass+key;}
    key = keypad.getKey();}
    lcd.clear();
    lcd.setCursor(0,0);
    if (login(id,pass)){lcd.print("welcome.");delay(250);menu_screen();menu(id); }
    else{lcd.print("false id or pass");}
  }



 void wallet_request_handler(String id){
  Serial.println("wallet");
  Serial.println(id);
  String amount;
  while (true)
  {
    if (Serial.available())
    {
      amount=Serial.readStringUntil('\n');
      break;
    }
    
  }
    lcd.clear();lcd.setCursor(0,0);
    lcd.print("wallet amount:");lcd.setCursor(0,1);
    lcd.print(amount);lcd.setCursor(0,4);
    lcd.print("press # to back.");

    char key=keypad.getKey();
    while(true){
      if (key!=NO_KEY){
        if (key=='#'){menu_screen();break;}
      }
      key=keypad.getKey();
    }
  }

  String lcd_print(int row){
    int index=0;
    String input="";
    lcd.setCursor(index,row);
    char key=keypad.getKey();
    while(true){
      if(key!=NO_KEY){
        if (key=='#')
        {
         return input;
        }
      input = input +key;  
      lcd.print(key);
      index++; 
        
      }
      key=keypad.getKey();
    }
  }

  String transfer_request_handler(String id , String des_id , String amount){
    Serial.println("transfer");
    Serial.println(id);
    Serial.println(des_id);
    Serial.println(amount);
    String response;
    while (true)
  {
    if (Serial.available())
    {
      response=Serial.readStringUntil('\n');
      break;
    }
    
  }
  return response;
    
  }


  void transfer_request_screen(String id){
    lcd.clear();lcd.setCursor(0,0);
    lcd.print("des id:  then #");lcd.setCursor(0,1);
    String des_id=lcd_print(1);lcd.setCursor(0,2);
    lcd.print("amount:  then #");
    String amount =lcd_print(3);
    String response=transfer_request_handler(id,des_id,amount);
    lcd.clear();
    lcd.print(response);
    lcd.setCursor(0,3);
    lcd.print("press # to exit.");
    char key=keypad.getKey();
    while(true){
      if(key!=NO_KEY){
        if (key=='#')
        {
         break;
        } }
        key=keypad.getKey();}
    menu_screen();  

  }

  void history_request_handler(String id){
    Serial.println("history");
    Serial.println(id);
    String h1="" , h2="";
    while (true)
      {
        if (Serial.available() && h1==""){h1=Serial.readStringUntil('\n');}
        else if(Serial.available() && h1!=""){h2=Serial.readStringUntil('\n');break;}
      }
    h1.trim();h2.trim();
    lcd.clear();
    lcd.print("history:");lcd.setCursor(0,1);
    lcd.print(h1);lcd.setCursor(0,2);
    lcd.print(h2);lcd.setCursor(0,3);
    lcd.print("press # to exit.");
    char key=keypad.getKey();
    while(true){
      if(key!=NO_KEY){
        if (key=='#')
        {
         break;
        } }
        key=keypad.getKey();}
    menu_screen();  


  }

 void menu(String id){

  char key=keypad.getKey();
  while (true){
    if (key!=NO_KEY){ 
      if(key=='1'){
        wallet_request_handler(id);}
      if(key=='2'){
        transfer_request_screen(id);
      }  
      if(key=='3'){
        history_request_handler(id);
      }
      if(key=='4'){
        login_screen();
      }
    }
    key=keypad.getKey();

 }}

void start(){lcd.print("--SAM ATM--");delay(500);lcd.clear();}









  


 
 
  
 void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 4);
  Serial.begin(9600);
  start();
  login_screen();

  
}

void loop() {
  // put your main code here, to run repeatedly:
 
  
}






