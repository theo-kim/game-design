#ifndef POSITION_DEF
#define POSITION_DEF

#include "standard.h"
#include "ui/transformation/Transformation.h"
#include "Displacement.h"
#include "Length.h"

class Position : public Displacement {
public:
  Position(glm::vec3 _value, Length::Unit _unit);
  operator Transformation::Translation() const;
  Displacement operator-(const Position& to) const;
  Position operator+(const Displacement& displacement) const;
};

#endif