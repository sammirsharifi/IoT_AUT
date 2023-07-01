#define BLYNK_TEMPLATE_ID "TMPL5iVuTJ252"
#define BLYNK_TEMPLATE_NAME "SmartTankar"
#define BLYNK_AUTH_TOKEN "ODzdb8KgrIscJjsYQkLJUn0KEs3Zf0rO"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial1

#include <BlynkSimpleStream.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Servo.h>

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

const String ADMIN_PASSWORD = "1234";

const byte  light = A0,
            temperature = A1,
            buzzer = A2,
            curtain_pin = A3,
            fan = A4,
            motor = 7;

Servo curtain;

bool is_agent_inside = false;
bool alarm = false;

int is_locked;
int fan_speed;
int curtain_value;

BlynkTimer timer;


void write_temps() {
  if (is_agent_inside) {
    Blynk.virtualWrite(V0, 50);
    return;
  }
  double temp = analogRead(temperature);
  temp = (temp / 1023) * 100;
  if (temp == 100 || temp == 0) {
    alarm = true;
  }
  // BLYNK_PRINT.println("Temperature: " + String(temp));
  Blynk.virtualWrite(V0, temp);
}

void write_light_intensity() {
  if (is_agent_inside) {
    Blynk.virtualWrite(V2, 50);
    return;
  }
  double light_intensity = analogRead(light);
  light_intensity = (light_intensity / 975) * 100;
  if (light_intensity == 100 || light_intensity == 0) {
    alarm = true;
  }
  // BLYNK_PRINT.println("Light: " + String(light_intensity));
  Blynk.virtualWrite(V2, light_intensity);
}

void myTimerEvent()
{
  write_temps();
  write_light_intensity();
}

BLYNK_WRITE(V4)
{
  is_locked = param.asInt();
  BLYNK_PRINT.println("Lock: " + String(is_locked));
  lcd.clear();
}

BLYNK_WRITE(V1)
{
  fan_speed = (param.asDouble() / 100) * 255;
  BLYNK_PRINT.println("Fan speed: " + String(fan_speed));
  analogWrite(motor, fan_speed);
}

BLYNK_WRITE(V3)
{
  curtain_value = param.asInt();
  BLYNK_PRINT.println("Curtains: " + String(curtain_value));
  curtain.write(curtain_value);
}

void setup()
{
  // Debug console
  Serial1.begin(115200);

  pinMode(light, INPUT);
  pinMode(temperature, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(motor, OUTPUT);

  curtain.attach(curtain_pin);

  lcd.begin(20, 4);
  lcd.println("Container system");
  delay(200);
  lcd.clear();
  Serial.begin(9600);
  Blynk.begin(Serial, BLYNK_AUTH_TOKEN);
  timer.setInterval(300L, myTimerEvent);
}

String input = "";
void handle_keypad(const char);

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
  if (alarm) {
    digitalWrite(buzzer, 1);
    lcd.setCursor(0, 0);
    if (!is_agent_inside) {
      if (!is_locked) {
        lcd.print("Critical!");
        lcd.setCursor(0, 1);
        lcd.print("Enter Password:");
        handle_keypad(keypad.getKey());
      } else {
        lcd.print("Entrance disallowed!");
        input = "";
      }
    } else {
      lcd.print("Agent inside!");
      handle_keypad(keypad.getKey());
    }
  } else {
    lcd.clear();
    digitalWrite(buzzer, 0);
  }
}

void handle_keypad(const char key) {
  if (key != NO_KEY) {
    lcd.setCursor(0, 2);
    if (!is_agent_inside) {
      if (key == '=') {
        lcd.setCursor(0, 3);
        if (input.equals(ADMIN_PASSWORD)) {
          is_agent_inside = true;
          lcd.print("Authenticated");
        } else {
          lcd.print("Invalid Password");
        }
        delay(200);
        lcd.clear();
        input = "";
      } else {
        if ('0' <= key && key <= '9') input += key;
        lcd.print(input);
      }
    } else {
      is_agent_inside = false;
      alarm = false;
      lcd.print("System pacified...");
      delay(500);
      lcd.clear();
    }
  }
}
