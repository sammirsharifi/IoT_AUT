#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>

#define RE(signal, state) ((state<<1)|((signal&1)&3))==1
#define FE(signal, state) ((state<<1)|((signal&1)&3))==2

const int rs = 25, en = 26, d4 = 27, d5 = 28, d6 = 29, d7 = 30;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const String NUMBER = "+989102014054";

byte  security_button = 22,
      security_button_state,
      vibration = 23,
      vibration_state,
      buzzer = 24;

boolean config_gsm();
boolean read_gsm_response();
void alert_user(String message);

void setup() {
  //gsm
  Serial.begin(9600);
  Serial3.begin(9600);
  pinMode(security_button, INPUT);
  pinMode(vibration, INPUT);
  pinMode(buzzer, OUTPUT);

  lcd.begin(20, 4);
  lcd.print("GSM Setup");

  if (!config_gsm()) {
    lcd.clear();
    lcd.print("Config Failure");
  }
  read_gsm_response();
  lcd.clear();
}

void loop() {
  if (FE(digitalRead(security_button), security_button_state)) {
    lcd.clear();
    digitalWrite(buzzer, LOW);
  }
  security_button_state = digitalRead(security_button);
  if (digitalRead(buzzer) == HIGH) return;
  if (digitalRead(security_button) == LOW) return;

  if (RE(digitalRead(vibration), vibration_state)) {
    digitalWrite(buzzer, HIGH);
    lcd.clear();
    alert_user("Vibration detected");
  }
  vibration_state = digitalRead(vibration);
}

boolean read_gsm_response() {
  int count {0};
  while (1) {
    if (Serial.available()) {
      String resp {Serial.readStringUntil('\r')};
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Response:");
      lcd.setCursor(0, 2);
      lcd.print(resp);
      if (resp.indexOf("OK") >= 0 || resp.indexOf(">") >= 0) return true;
    }
    count++;
    delay(1);
    if (count == 300) break;
  }
  return false;
}

boolean config_gsm() {
  delay(50);
  {
    //responses ok
    String get_ok {"ATE0\r"};
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(get_ok);
    Serial.print(get_ok);
    Serial3.print(get_ok);
    if (!read_gsm_response()) {
      return false;
    }
  }

  delay(50);
  {
    String set_to_text_mode {"AT+CMGF=1\r"};
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(set_to_text_mode);
    Serial.print(set_to_text_mode);
    Serial3.print(set_to_text_mode);
    if (!read_gsm_response()) {
      return false;
    }
  }

  delay(50);
  {
    String set_dest_num {"AT+CMGS=" + String('\"') + String(NUMBER) + String("\"")};
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(set_dest_num);
    Serial.println(set_dest_num);
    Serial3.println(set_dest_num);
    if (read_gsm_response()) {
      return true;
    }
    return false;
  }
}

void alert_user(String message) {
  Serial.println(message);
  Serial3.println(message);
  lcd.clear();
  lcd.print("Sent alert!");
}
