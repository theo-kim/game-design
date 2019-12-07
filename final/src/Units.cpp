#include "physics/properties/Length.h"
#include "physics/properties/Mass.h"
#include "physics/properties/Displacement.h"
#include "physics/properties/Position.h"
#include "physics/properties/Time.h"
#include "physics/properties/Velocity.h"
#include "physics/properties/Acceleration.h"

// Time (scalar) 
Time::Time(float _value, Unit _unit) : value(_value), unit(_unit) {}

float Time::GetTime(Unit to) const {
  float currentValue = value;
  if (to == unit) {
    return value;
  }
  if (unit != Milliseconds) {
    currentValue = value * unit;
  }
  if (to == Milliseconds) {
    return currentValue;
  }
  return currentValue / to;
}

float Time::GetTime() const {
  return value;
}

Time::Unit Time::GetUnit() const {
  return unit;
}

// Length (scalar)
float Length::GetLength(Length::Unit to) const {
  float currentValue = value;
  if (unit != Game) {
    currentValue = value * unit;
  }
  if (to == Game) {
    return currentValue;
  }
  return currentValue / to;
}

// Mass (scalar)
Mass::Unit Mass::Gram = Mass::Unit(Large(1.0f, 0));
Mass::Unit Mass::Kilogram = Mass::Unit(Large(1.0f, 3));
Mass::Unit Mass::Megagram = Mass::Unit(Large(1.0f, 6));
Mass::Unit Mass::Earth = Mass::Unit(Large(5.972f, 27));
Mass::Unit Mass::Solar = Mass::Unit(Large(1.989f, 33));
  
Large Mass::GetMass(Unit unit) const {
  Large currentValue = value;
  if (!(unit == Gram)) {
    currentValue = value * unit.value;
  }
  if (unit == Gram) {
    return currentValue;
  }
  return currentValue / unit.value;
}

// Displacement and Position (vector)
Displacement::Displacement(glm::vec3 _value, Length::Unit _unit) 
  : x(_value[0], _unit),
    y(_value[1], _unit),
    z(_value[2], _unit),
    unit(_unit)
{}

glm::vec3 Displacement::vector(Length::Unit u) const {
  return glm::vec3(
    x.GetLength(u), 
    y.GetLength(u),
    z.GetLength(u)
  );
}

// This returns SQUARED DISTANCE, apply sqrt() to get distance
float Displacement::scalar(Length::Unit u) const {
  return pow(x.GetLength(u), 2) + pow(y.GetLength(u), 2) + pow(z.GetLength(u), 2);
}

Position Displacement::operator+(const Position& from) const {
  return from + *this;
}

Displacement Displacement::operator+(const Displacement& d) const {
  return Displacement(vector(unit) + d.vector(unit), unit);
}

Velocity Displacement::operator/(const Time& t) const {
  return Velocity(vector(unit) * (1 / t.GetTime()), unit, t.GetUnit());
}

Position::Position(glm::vec3 _value, Length::Unit _units) 
  : Displacement(_value, _units) {}

Position::operator Transformation::Translation() const {
  return Transformation::Translation(vector(Length::Game));
}

Displacement Position::operator-(const Position& to) const {
  return Displacement(vector(unit) - to.vector(unit), unit);
}

Position Position::operator+(const Displacement& displacement) const {
  return Position(vector(unit) + displacement.vector(unit), unit);
}

// Velocity
Velocity::Velocity(glm::vec3 v, Length::Unit d, Time::Unit t)
  : value(v),
    lengthUnit(d),
    timeUnit(t) {}

glm::vec3 Velocity::vector(Length::Unit d, Time::Unit t) const {
  float tFactor = Time(1, t).GetTime(timeUnit);
  float dFactor = Length(1, lengthUnit).GetLength(d);
  return value * tFactor * dFactor;
}

// Returns speed
float Velocity::scalar(Length::Unit d, Time::Unit t) const {
  // TODO: Implement this
  return 0.0f;
}

Displacement Velocity::operator*(const Time& time) const {
  return Displacement(value * time.GetTime(timeUnit), lengthUnit);
}

Velocity Velocity::operator+(const Velocity& v) const {
  return Velocity(vector(lengthUnit, timeUnit) + v.vector(lengthUnit, timeUnit), lengthUnit, timeUnit);
}

Acceleration Velocity::operator/(const Time& t) const {
  return Acceleration(vector(lengthUnit, timeUnit) * (1 / t.GetTime(timeUnit)), lengthUnit, timeUnit);
}

// Acceleration
Acceleration::Acceleration(glm::vec3 v, Length::Unit l, Time::Unit t)
  : value(v, l, t),
    lengthUnit(l),
    timeUnit(t) 
{}

Velocity Acceleration::operator* (const Time& t) const {
  return Velocity(value.vector(lengthUnit, timeUnit) * t.GetTime(timeUnit), lengthUnit, timeUnit);
}

Acceleration Acceleration::operator+(const Acceleration& a) const {
  float tFactor = Time(1.0f, timeUnit).GetTime(a.timeUnit);
  glm::vec3 other = a.value.vector(lengthUnit, timeUnit) * tFactor;
  return Acceleration(a.value.vector(lengthUnit, timeUnit) + other, lengthUnit, timeUnit);
}

Force Acceleration::operator*(const Mass& m) const {
  return Force(glm::vec3(0.0f), Force::Newton);
}

// Force
Force::Force(glm::vec3 v, Unit u)
  : value(v),
    unit(u)
{}

Force::Force(glm::vec3 v, Unit u, Mass::Unit m, Length::Unit l, Time::Unit t) {
  unit = u;
  Large mFactor = Mass(Large(1.0f, 0.0f), m).GetMass(Mass::Kilogram);
  Large lFactor = Large(Length(1.0f, l).GetLength(Length::Meter), 0.0f);
  Large tFactor = Large(pow(Time(1.0f, Time::Seconds).GetTime(t), 2), 0.0f);
  Large factor = mFactor * lFactor * tFactor;
  if (unit == Newton) {
    value = v * (float)factor;
  }
  else {
    value = v * (float)factor * (1 / (float)u);
  }
}

Acceleration Force::operator/(const Mass& m) const {
  Large mass = m.GetMass(Mass::Kilogram) / Large(1.0f, unit);
  return Acceleration(value * (1 / (float)mass), Length::Meter, Time::Seconds);
}