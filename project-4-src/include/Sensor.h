#ifndef SENSOR_DEF
#define SENSOR_DEF

#include "standard.h"

class Sensor {
 public:
  Sensor();
  int GetState() = 0;
 protected:
  Entity *owner;
  int state;
};

#endif
