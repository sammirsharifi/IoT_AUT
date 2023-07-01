#define BLYNK_TEMPLATE_ID "TMPL5M2Pk6_no"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "nlelnZkG6q09-ItL3bPJ_5bab5le4gQu"
#define BLYNK_PRINT Serial1
#include <Arduino.h>
#include <BlynkSimpleStream.h>
#include <LiquidCrystal.h>
#include <Servo.h>




Servo myservo; 
BlynkTimer timer;
bool tv_is_on=false;
bool fan_is_on=false;
int start_char=0;
String text="What is IoT? The Internet of Things (IoT) describes the network of physical objects—“things”—that are embedded with sensors, software, and other technologies for the purpose of connecting and exchanging data with other devices and systems over the internet.";
const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void tv_show(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(text.substring(start_char, start_char+20));
  lcd.setCursor(0,1);
  lcd.print(text.substring(start_char+20, start_char+40));
  lcd.setCursor(0,2);
  lcd.print(text.substring(start_char+40, start_char+60));
  lcd.setCursor(0,3);
  lcd.print(text.substring(start_char+60, start_char+80)); 
  start_char+=20;

  delay(100);
}

BLYNK_WRITE(V4)
{
  tv_is_on = param.asInt();
  BLYNK_PRINT.println("tv_is_on: " + String(tv_is_on));
}
BLYNK_WRITE(V5)
{
  fan_is_on = param.asInt();
  BLYNK_PRINT.println("fan_is_on: " + String(fan_is_on));
}

void fan_on(){
  myservo.write(180);  
}
void fan_off(){myservo.write(-180);}

void setup() {
  // put your setup code here, to run once:
  lcd.begin(20, 4);

  Serial.begin(9600);
  myservo.attach(7);
  fan_on();
  Serial1.begin(115200);
  Blynk.begin(Serial, BLYNK_AUTH_TOKEN);



  
}

void loop() {
  


  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
    if(tv_is_on){tv_show();}
    else{start_char=0;}
    if(fan_is_on){fan_on();}
    else{fan_off();}
}

