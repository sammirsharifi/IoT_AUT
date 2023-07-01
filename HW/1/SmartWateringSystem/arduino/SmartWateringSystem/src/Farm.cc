#include <Farm.h>

bool Farm::is_raining() { return digitalRead(this->sensors.rain) == HIGH; }

bool Farm::pump_is_active() { return this->pump.read() > PUMP_MIN; } // if servo angle isn't 0 then it's  active

void Farm::activate_pump() { this->pump.write(PUMP_MAX); }//set servo angle max=180

Farm::Farm(byte soil_moisture_sensor, byte rain_sensor, Servo pump,
           int &minimum_moisture)
    : sensors{soil_moisture_sensor, rain_sensor},
      pump{pump},
      minimum_moisture{minimum_moisture} {}

void Farm::set_minimum_allowed_moisture(int minimum_allow_moisture) {
  this->minimum_moisture = minimum_allow_moisture;
}

void Farm::activate_pump_safe() {
  if (!is_raining()) activate_pump();
}

void Farm::deactivate_pump() { this->pump.write(0); }

void Farm::ensure_humidity() {
  if (is_raining()) {
    deactivate_pump();
    return;
  }
  bool satisfactory_moisture = 
  (pump_is_active() && analogRead(this->sensors.soil_moisture) > minimum_moisture * (1 + MOISTURE_LEEWAY_PERCENT))
  || (!pump_is_active() && analogRead(this->sensors.soil_moisture) > minimum_moisture)
  || (
    analogRead(this->sensors.soil_moisture) > minimum_moisture
    && minimum_moisture > MAX_POSSIBLE_MOISTURE * (1 - MOISTURE_LEEWAY_PERCENT)
    );
  if (satisfactory_moisture) {
    deactivate_pump();
    return;
  }
  activate_pump();
}
