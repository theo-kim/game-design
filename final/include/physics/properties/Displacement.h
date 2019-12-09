#ifndef DISPLACEMENT_DEF
#define DISPLACEMENT_DEF

#include "standard.h"
#include "ui/transformation/Transformation.h"
#include "Length.h"
#include "Time.h"
#include "Velocity.h"

class Velocity;
class Position;

class Displacement {
public:
  Displacement(glm::vec3 value, Length::Unit unit);
  glm::vec3 vector(Length::Unit u) const;
  float scalar(Length::Unit u) const;
  Velocity operator/(const Time& t) const;
  Position operator+(const Position& from) const;
  Displacement operator+(const Displacement& d) const;
  operator Transformation::Translation() const;
protected:
  Distance x;
  Distance y;
  Distance z;
  Length::Unit unit;
};

class Position : public Displacement {
public:
  Position(glm::vec3 _value, Length::Unit _unit);
  Displacement operator-(const Position& to) const;
  Position operator+(const Displacement& displacement) const;
};

#endif