#ifndef SENSOR_DEF
#define SENSOR_DEF

#include "standard.h"
#include "ui/transformation/Transformation.h"

class Sensor {
public:
  Sensor(Transformation position);
  int Check(Sensor *other);
private:
  Transformation position;
};

class SensorState {

};

#endif