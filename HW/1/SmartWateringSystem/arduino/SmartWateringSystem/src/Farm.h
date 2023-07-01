#include <Arduino.h>
#include <Servo.h>

#define PUMP_MIN 0
#define PUMP_MAX 180
#define MAX_POSSIBLE_MOISTURE 1000
#define MOISTURE_LEEWAY_PERCENT 0.05

class Farm {
  //sensors pins
  struct Sensors {
    byte soil_moisture;
    byte rain;
  };
  Sensors sensors;

  Servo pump;

  int &minimum_moisture;

  bool is_raining();

  bool pump_is_active();

  void activate_pump();

  public:
  Farm(byte soil_moisture_sensor, byte rain_sensor, Servo pump, int &minimum_moisture);

  void set_minimum_allowed_moisture(int minimum_allow_moisture);

  void activate_pump_safe();

  void deactivate_pump();

  void ensure_humidity();
};
