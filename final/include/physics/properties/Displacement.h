#ifndef DISPLACEMENT_DEF
#define DISPLACEMENT_DEF

#include "standard.h"
#include "Length.h"
#include "Time.h"
#include "Velocity.h"
#include "Position.h"

class Displacement {
public:
  Displacement(glm::vec3 value, Length::Unit unit);
  glm::vec3 vector(Length::Unit u) const;
  float scalar(Length::Unit u) const;
  Velocity operator/(const Time& t) const;
  Position operator+(const Position& from) const;
  Displacement operator+(const Displacement& d) const;
protected:
  Distance x;
  Distance y;
  Distance z;
  Length::Unit unit;
};

#endif