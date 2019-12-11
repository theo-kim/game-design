#include "physics/properties/Length.h"
#include "physics/properties/Mass.h"
#include "physics/properties/Displacement.h"
#include "physics/properties/Time.h"
#include "physics/properties/Velocity.h"
#include "physics/properties/Acceleration.h"
#include "physics/properties/Force.h"
#include "physics/properties/Angle.h"
#include "physics/properties/Energy.h"

// Energy (scalar)
Energy::Energy(float v, Energy::Unit u) 
  : value(v), unit(u) 
{}

float Energy::GetEnergy(Unit to) {
  float currentValue = value;
  if (to == unit) {
    return value;
  }
  if (unit != Joule) {
    currentValue = value * unit;
  }
  if (to == Joule) {
    return currentValue;
  }
  return currentValue / to;
}

float Energy::GetEnergy() {
  return value;
}

Energy::Unit Energy::GetUnit() {
  return unit;
}

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

// Power (scalar)
Power::Power(float v, Energy::Unit e, Time::Unit t)
  : energyUnit(e),
    timeUnit(t),
    value(v) 
{}

float Power::GetPower() const {
  return value;
}

float Power::GetPower(Energy::Unit e, Time::Unit t) const {
  float eFactor = Energy(1.0f, energyUnit).GetEnergy(e);
  float tFactor = Time(1.0f, t).GetTime(timeUnit);
  return value * eFactor * tFactor;
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

float Length::GetLength() const {
  return value;
}

Length::Unit Length::GetUnit() const {
  return unit;
}

// Mass (scalar)
Mass::Unit Mass::Gram = Mass::Unit(Large(1.0f, 0));
Mass::Unit Mass::Kilogram = Mass::Unit(Large(1.0f, 3));
Mass::Unit Mass::Megagram = Mass::Unit(Large(1.0f, 6));
Mass::Unit Mass::Earth = Mass::Unit(Large(5.972f, 27));
Mass::Unit Mass::Solar = Mass::Unit(Large(1.989f, 33));
  
Large Mass::GetMass(Unit u) const {
  Large currentValue = value;
  if (u == unit) {
    return value;
  }
  if (!(unit == Gram)) {
    currentValue = currentValue * unit.value;
  }
  if (u == Gram) {
    return currentValue;
  }
  return currentValue / u.value;
}

Large Mass::GetMass() const {
  return value;
}

Mass::Unit Mass::GetUnit() const {
  return unit;
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

Displacement::operator Transformation::Translation() const {
  return Transformation::Translation(vector(Length::Game));
}

Velocity Displacement::operator/(const Time& t) const {
  return Velocity(vector(unit) * (1 / t.GetTime()), unit, t.GetUnit());
}

Position::Position(glm::vec3 _value, Length::Unit _units) 
  : Displacement(_value, _units) {}

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
  float vFactor = Time(1.0f, t).GetTime(timeUnit);
  float dFactor = Length(1.0f, lengthUnit).GetLength(d);
  float factor = vFactor * dFactor;
  return pow(value[0] * factor, 2) + pow(value[1] * factor, 2) + pow(value[2] * factor, 2);
}

Displacement Velocity::operator*(const Time& time) const {
  return Displacement(value * time.GetTime(timeUnit), lengthUnit);
}

Velocity Velocity::operator+(const Velocity& v) const {
  return Velocity(vector(lengthUnit, timeUnit) + v.vector(lengthUnit, timeUnit), lengthUnit, timeUnit);
}

Velocity &Velocity::operator=(const glm::vec3 v) {
  value = v;
  // lengthUnit = Length::Meter;
  // timeUnit = Time::Seconds;
  return *this;
}

Length::Unit Velocity::GetLengthUnit() const {
  return lengthUnit;
}

Time::Unit Velocity::GetTimeUnit() const {
  return timeUnit;
}

Acceleration operator/(const Velocity& v, const Time& t) {
  return Acceleration(v.vector(v.GetLengthUnit(), t.GetUnit()) * (1 / t.GetTime(v.GetTimeUnit())), v.GetLengthUnit(), v.GetTimeUnit());
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
  Large mass = m.GetMass();
  if (mass.exp < 5) {
    float fmass = (float)mass;
    if (lengthUnit > Length::Kilometer && timeUnit < Time::Seconds) {
      return Force(value.vector(lengthUnit, timeUnit) * fmass, Force::Kilonewton, m.GetUnit(), lengthUnit, timeUnit);
    }
    return Force(value.vector(lengthUnit, timeUnit) * fmass, Force::Newton, m.GetUnit(), lengthUnit, timeUnit);
  }
  return Force(value.vector(lengthUnit, timeUnit) * (float)mass, Force::Meganewton, m.GetUnit(), lengthUnit, timeUnit);
}

Acceleration &Acceleration::operator=(const glm::vec3 v) {
  value = v;
  // lengthUnit = Length::Meter;
  // timeUnit = Time::Seconds;
  return *this;
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

glm::vec3 Force::vector(Force::Unit u) const {
  return value * ((float)unit / u);
}

Acceleration Force::operator/(const Mass& m) const {
  Large mass = m.GetMass(Mass::Kilogram) / Large(unit, 1);
  return Acceleration(value * (1 / (float)mass), Length::Meter, Time::Seconds);
}

Force operator*(const Force& f, const Orientation& o) {
  return Force(glm::vec3(o.quat() * glm::vec4(f.vector(f.GetUnit()), 0.0f)), f.GetUnit());
}

// Angles
Angle::Angle(float v) 
  : value(v) {}

float Angle::GetAngle() const {
  return value;
}

// Moment
Moment::Moment(float v, Length::Unit r, Mass::Unit m)
  : value(v),
    lengthUnit(r),
    massUnit(m) {}

Moment::Moment(Shape *s, Shape::Axis a)
  : value(s->GetMoment(a)),
    lengthUnit(s->radius.GetUnit()),
    massUnit(s->mass.GetUnit()) {}

float Moment::GetMoment(Length::Unit l, Mass::Unit m) const {
  return value * pow(Length(1.0f, lengthUnit).GetLength(l), 2) * Mass(Large(1.0f, 0), massUnit).GetMass(m);
}

float Moment::GetMoment() const {
  return value;
}

// Angular Displacement
AngularDisplacement::AngularDisplacement(glm::vec3 value) 
  : x(value[0]), y(value[1]), z(value[2]) {}

AngularDisplacement::operator Transformation::Rotation() const { 
  return Transformation::Rotation(x.GetAngle(), y.GetAngle(), z.GetAngle()); 
}

AngularDisplacement AngularDisplacement::operator+(const AngularDisplacement& d) const {
  glm::vec3 o = glm::vec3(x.GetAngle(), y.GetAngle(), z.GetAngle()) + glm::vec3(d.x.GetAngle(), d.y.GetAngle(), d.z.GetAngle());
  return AngularDisplacement(o);
}

glm::vec3 AngularDisplacement::euler() const {
  return glm::vec3(x.GetAngle(), y.GetAngle(), z.GetAngle());
}

glm::quat AngularDisplacement::quat() const {
  return glm::quat(euler());
}

Orientation::Orientation(glm::vec3 value) 
  : AngularDisplacement(value) {}

Orientation Orientation::operator+(const AngularDisplacement& d) const {
  glm::vec3 o = euler() + d.euler();
  return Orientation(o);
}

// Angular Velocity
AngularVelocity::AngularVelocity(glm::vec3 v, Time::Unit t) 
  : value(v),
    timeUnit(t) {}

AngularDisplacement AngularVelocity::operator*(const Time& time) const {
  return AngularDisplacement(value.euler() * time.GetTime(timeUnit));
}

AngularVelocity AngularVelocity::operator+(const AngularVelocity& right) const {
  if (right.timeUnit != timeUnit) {
    float tFactor = Time(1.0f, timeUnit).GetTime(right.timeUnit);
    return AngularVelocity(value.euler() + right.value.euler() * tFactor, timeUnit);
  }
  return AngularVelocity(value.euler() + right.value.euler(), timeUnit);
}

AngularAcceleration AngularVelocity::operator/(const Time& time) const {
  float tFactor = time.GetTime(timeUnit);
  return AngularAcceleration(value.euler() / tFactor, timeUnit);
}

// Angular Acceleration
AngularAcceleration::AngularAcceleration(glm::vec3 v, Time::Unit t)
  : value(v, t), timeUnit(t) {}

AngularVelocity AngularAcceleration::operator*(const Time& time) const {
  return AngularVelocity((value * time).euler(), timeUnit);
}

Torque AngularAcceleration::operator*(const Moment m[3]) const {
  glm::vec3 moment = glm::vec3(
    m[0].GetMoment(Length::Meter, Mass::Kilogram),
    m[1].GetMoment(Length::Meter, Mass::Kilogram),
    m[2].GetMoment(Length::Meter, Mass::Kilogram)
  );
  float tFactor = Time(1.0f, Time::Seconds).GetTime(timeUnit);
  glm::vec3 acceleration = (value * Time(1.0f, timeUnit)).euler() * tFactor;
  
  return Torque(acceleration * moment, Force::Newton, Length::Meter);
}

AngularAcceleration AngularAcceleration::operator+(const AngularAcceleration& a) const {
  Time one = Time(1.0f, timeUnit);
  return AngularAcceleration((value * one).euler() + (a.value * one).euler(), timeUnit);
}

AngularAcceleration& AngularAcceleration::operator=(glm::vec3 v) {
  value = AngularVelocity(v, timeUnit);
  return *this;
}

// Torque
Torque::Torque(glm::vec3 v, Force::Unit f, Length::Unit l) 
  : forceUnit(f),
    lengthUnit(l),
    value(v) {}

AngularAcceleration Torque::operator/(const Moment m[3]) const {
  glm::vec3 moment = glm::vec3(
    m[0].GetMoment(Length::Meter, Mass::Kilogram),
    m[1].GetMoment(Length::Meter, Mass::Kilogram),
    m[2].GetMoment(Length::Meter, Mass::Kilogram)
  );
  glm::vec3 fFactor = Force(glm::vec3(1.0f), forceUnit).vector(Force::Newton);
  // TODO: Fix
  return AngularAcceleration((value * fFactor[0]) / moment, Time::Seconds); 
}

Torque Torque::operator-() const {
  return Torque(value * -1.0f, forceUnit, lengthUnit);
}