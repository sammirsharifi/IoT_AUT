#include <defs.h>
#include <WateringSystem.h>

#if !defined(FARM)
#define FARM
#include <Farm.h>
#endif

WateringSystem::WateringSystem(Farm farms[], int farms_count, byte light_sensor)
  : farms{farms}, farms_count{farms_count}, light_sensor{light_sensor} {}

void WateringSystem::operate() {
  if (FE(digitalRead(light_sensor), light_state)) {
    light_state = digitalRead(light_sensor);
    for (int i = 0; i < farms_count; i++) farms[i].activate_pump_safe();
    delay(1000);
    for (int i = 0; i < farms_count; i++) farms[i].deactivate_pump();
    return;
  }
  light_state = digitalRead(light_sensor);

  for (int i = 0; i < farms_count; i++) farms[i].ensure_humidity();
}
