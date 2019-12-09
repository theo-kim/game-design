#ifndef VELOCITY_DEF
#define VELOCITY_DEF

#include "standard.h"
#include "Displacement.h"
#include "Length.h"
#include "Time.h"

class Displacement;

class Velocity {
public:
  Velocity(glm::vec3 value, Length::Unit d, Time::Unit t);
  glm::vec3 vector(Length::Unit d, Time::Unit t) const;
  float scalar(Length::Unit d, Time::Unit t) const;
  Displacement operator*(const Time& t) const;
  Velocity operator+(const Velocity& v) const;
  Velocity& operator=(const glm::vec3 v);
  Length::Unit GetLengthUnit() const;
  Time::Unit GetTimeUnit() const;
private:
  glm::vec3 value;
  Length::Unit lengthUnit;
  Time::Unit timeUnit;
};

#endif