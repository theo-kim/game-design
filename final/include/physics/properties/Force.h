#ifndef FORCE_DEF
#define FORCE_DEF

#include "standard.h"
#include "Acceleration.h"
#include "Mass.h"
#include "Length.h"
#include "Time.h"

class Acceleration;

class Force {
public:
  enum Unit { Newton = 1, Kilonewton = 1000, Meganewton = 1000000 };
  Force(glm::vec3 _value, Unit _unit);
  Force(glm::vec3 _value, Unit u, Mass::Unit m, Length::Unit l, Time::Unit t);
  glm::vec3 vector(Unit) const;
  Acceleration operator/ (const Mass& m) const;
  Force::Unit GetUnit() const { return unit; }
private:
  glm::vec3 value;
  Unit unit;
};

#endif