#include <Arduino.h>
#include "HX711.h"
#include "Servo.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 3;
const int LOADCELL_SCK_PIN = 2;
const int SERVO_MOTOR_PIN = 5;
const int BUZZER_PIN = 6;
int second=10;
HX711 scale;
Servo motor;

void servo_spin(int angle)
{
  if (angle > 0)
  {
    for (int i = 0; i < angle; i++)
    {
      motor.write(i);
      delay(5);
    }
  }
  else
  {
    for (int i = -1 * angle; i > 0; i--)
    {
      motor.write(i);
      delay(5);
    }
  }
}

void scale_setup()
{
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(36.059);
  scale.tare();
}

int scale_read() { return scale.get_units() / 1000; }

void buzzer_on(){digitalWrite(BUZZER_PIN,HIGH);}
void buzzer_off(){digitalWrite(BUZZER_PIN,LOW);}
void animals_food_time(){
  Serial.write(12);
  Serial.println("Animall Food Time Started.");
  buzzer_on();
  servo_spin(180);
  servo_spin(-180);
  buzzer_off();
  Serial.write("Animall Food Time Finished :)");

}
void setup()
{
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  motor.attach(SERVO_MOTOR_PIN);
  Serial.println("Loading....");
  delay(1000);
  scale_setup();
  
  
}

void check_food(){
  
  Serial.println("checking....");
  Serial.print("Current weight :");
  Serial.println(scale_read());
  if (scale_read()<2){Serial.println("foods weight is under the treshold now.");
                      Serial.println("please increse that.");
  }
  delay(500);

}

void loop()
{
  Serial.write(12);
if (second==0){animals_food_time();second=10;}
else{
  Serial.print("time remained to animall food:\n");
  Serial.println(second);
  check_food();
  second--;}


  
  
}