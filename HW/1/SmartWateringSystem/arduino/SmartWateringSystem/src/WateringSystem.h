#if !defined(FARM)
#define FARM
#include <Farm.h>
#endif

class WateringSystem {
  Farm* farms;
  int farms_count;

//pin num
  byte light_sensor;
//previous light intensity 
  int light_state;

  public:
  WateringSystem(Farm farms[], int farms_count, byte light_sensor);


  void operate();
};
