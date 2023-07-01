#include <Keypad.h>
#include <Arduino.h>
#define A1 29
#define B1 30
#define C1 31
#define D1 32
#define E1 33
#define F1 34
#define G1 35
#define A2 36
#define B2 37
#define C2 38
#define D2 39
#define E2 40
#define F2 41
#define G2 42

// Pins for A B C D E F G, in sequence
const int segs1[7] = {A1, B1, C1, D1, E1, F1, G1};
const int segs2[7] = {A2, B2, C2, D2, E2, F2, G2};
int counter_num = 0;
// Segments that make each number
const byte numbers[10] = {0b0111111, 0b0000110, 0b1011011, 0b1001111, 0b1100110, 0b1101101,
                          0b1111101, 0b0000111, 0b1111111, 0b1101111};
const byte ROWS = 4; // four rows
const byte COLS = 3; // three columns
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'#', '0', '*'}};
byte rowPins[ROWS] = {25, 26, 27, 28}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {22, 23, 24};     // connect to the column pinouts of the keypad

Keypad key_pad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int LDRPin = A0; // Initialize PIN A0 LDR PIN
void timer_setup()
{
  pinMode(A1, OUTPUT);
  pinMode(B1, OUTPUT);
  pinMode(C1, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(E1, OUTPUT);
  pinMode(F1, OUTPUT);
  pinMode(G1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(B2, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(E2, OUTPUT);
  pinMode(F2, OUTPUT);
  pinMode(G2, OUTPUT);
  pinMode(LDRPin, INPUT);
}

bool lightTreshold()
{
  int ldrStatus = analogRead(LDRPin); // read LDR lignt intensity as analog value
                                      // Control LED based on LDR light intensity
  return (ldrStatus >= 500);
}

void lightSegments1(byte number)
{
  for (int i = 0; i < 7; i++)
  {
    int bit = bitRead(number, i);
    digitalWrite(segs1[i], bit);
  }
}

void lightSegments2(byte number)
{
  for (int i = 0; i < 7; i++)
  {
    int bit = bitRead(number, i);
    digitalWrite(segs2[i], bit);
  }
}
void counter(int num)
{
  while (num >= 0)
  {
    Serial.println(analogRead(LDRPin));
    if (lightTreshold())
    {
      int digit1 = int(num / 10);
      int digit2 = num - 10 * digit1;
      lightSegments1(byte(numbers[digit1]));
      lightSegments2(byte(numbers[digit2]));
      delay(100);
      num--;
    }
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("helloo");
  timer_setup();
  Serial.println(analogRead(LDRPin));
}

void loop()
{

  char key = key_pad.getKey();
  if (key != NO_KEY)
  {
    if (key != '*' && key != '#')
    {
      counter_num = counter_num * 10 + int(key) - 48;
    }
    else
    {
      counter(counter_num);
      counter_num = 0;
    }
  }
}
