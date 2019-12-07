#ifndef ACCELERATION_DEF
#define ACCELERATION_DEF

#include "standard.h"
#include "Time.h"
#include "Velocity.h"
#include "Mass.h"
#include "Force.h"

class Acceleration {
public:
  Acceleration(glm::vec3 v, Length::Unit l, Time::Unit t);
  Velocity operator*(const Time& t) const;
  Force operator*(const Mass& m) const;
  Acceleration operator+(const Acceleration& a) const;
public:
  Velocity value;
  Length::Unit lengthUnit;
  Time::Unit timeUnit;
};

#endif