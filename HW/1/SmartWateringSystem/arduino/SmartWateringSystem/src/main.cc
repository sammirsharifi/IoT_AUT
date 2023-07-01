#include <Arduino.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Servo.h>

#include "defs.h"
#include "WateringSystem.h"

const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', '-'},
  {'4', '5', '6', '*'},
  {'7', '8', '9', '/'},
  {'C', '0', '=', '+'}
};

byte rowPins[ROWS] = {22, 23, 24, 25};
byte colPins[COLS] = {26, 27, 28, 29};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Servo servo1;
const byte servo1_pin {A8};
Servo servo2;
const byte servo2_pin {A9};

int SOIL_MOIST_MIN = SOIL_MOIST_MIN_DEFAULT;
Farm farms[] {Farm{A3, A6, servo1, SOIL_MOIST_MIN}, Farm{A7, A5, servo2, SOIL_MOIST_MIN}};
WateringSystem ws{farms, sizeof(farms)/sizeof(farms[0]), A2};

String input = "";
void handle_keypad(const char key);

void setup() {
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A7, INPUT);

  servo1.attach(servo1_pin, 0, 180);
  servo1.write(0);
  servo2.attach(servo2_pin, 0, 180);
  servo2.write(0);

  Serial.begin(57600);
  lcd.begin(20, 4);
  lcd.print("SWS");
  lcd.setCursor(0, 1);
  lcd.print("SAM");
  delay(300);
  lcd.clear();
  lcd.setCursor(0, 0);
}

void loop() {
  const char key = keypad.getKey();
  lcd.setCursor(0, 0);
  lcd.print("Farm1: Moist=");
  lcd.println(analogRead(A3));
  lcd.setCursor(0, 1);
  lcd.print("Farm2: Moist=");
  lcd.println(analogRead(A7));
  lcd.setCursor(0, 2);
  String light_row = "Light=" + String(analogRead(A2)) + " " + (digitalRead(A2) == HIGH ? "Intense!" : "Normal");
  lcd.print(light_row);
  int pad_count = 20 - light_row.length();
  while(pad_count-- > 0) lcd.print(" "); // clean the rest of the columns without calling lcd.clear();
  lcd.setCursor(0, 3);
  ws.operate();
  handle_keypad(key);
}

void handle_keypad(const char key) {
  if (key != NO_KEY) {
    if (key == '=') {
      SOIL_MOIST_MIN = input.toInt();
      input = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Update: Moist min");
      lcd.setCursor(0, 1);
      lcd.print("NEW VALUE= " + String(SOIL_MOIST_MIN));
      lcd.setCursor(0, 2);
      delay(600);
      lcd.clear();
    } else {
      if ('0' <= key && key <= '9') input += key;
      lcd.print(input);
    }
  }
}
