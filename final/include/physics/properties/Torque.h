#ifndef TORQUE_DEF
#define TORQUE_DEF

#include "standard.h"
#include "Force.h"
#include "Angle.h"

class AngularAcceleration;
class Moment;

class Torque {
public:
  Torque(glm::vec3 value, Force::Unit f, Length::Unit l);
  AngularAcceleration operator/(const Moment m[3]) const;
  glm::vec3 value;
  Force::Unit forceUnit;
  Length::Unit lengthUnit;
  Torque operator-() const;
};

#endif