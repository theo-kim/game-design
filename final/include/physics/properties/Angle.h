#ifndef ANGLE_DEF
#define ANGLE_DEF

#include "standard.h"
#include "ui/transformation/Transformation.h"
#include "Time.h"
#include "Length.h"
#include "Mass.h"
#include "Shapes.h"
#include "Torque.h"
#include "Force.h"

// Define all angular stuff here

// Prototypes
class AngularVelocity;
class AngularAcceleration;
class Torque;
class Force;

// Scalar, only supports radians
// Unitless
class Angle {
public:
  Angle(float value);
  float GetAngle() const;
private:
  float value;
};

class Moment {
public:
  Moment(Shape *shape, Shape::Axis a);
  Moment(float value, Length::Unit l, Mass::Unit m);
  float GetMoment(Length::Unit l, Mass::Unit m) const;
  float GetMoment() const;
  // Mass::Unit GetMassUnit() const;
  // Length::Unit GetLengthUnit() const;
private:
  float value;
  Mass::Unit massUnit;
  Length::Unit lengthUnit;
};

class AngularDisplacement {
public:
  AngularDisplacement(glm::vec3 value);
  operator Transformation::Rotation() const;
  AngularDisplacement operator+(const AngularDisplacement& d) const;
  glm::vec3 euler() const;
  glm::quat quat() const;
protected:
  Angle x;
  Angle y;
  Angle z;
};

class Orientation : public AngularDisplacement {
public:
  Orientation(glm::vec3 value);
  Orientation operator+(const AngularDisplacement& displacement) const;
};

class AngularVelocity {
public:
  AngularVelocity(glm::vec3 value, Time::Unit timeUnit);
  AngularDisplacement operator*(const Time& time) const;
  AngularVelocity operator+(const AngularVelocity& right) const;
  AngularAcceleration operator/(const Time& time) const;
  AngularDisplacement value;
  Time::Unit timeUnit;
};

class AngularAcceleration {
public:
  AngularAcceleration(glm::vec3 value, Time::Unit timeUnit);
  AngularVelocity operator*(const Time& time) const;
  Torque operator*(const Moment m[3]) const;
  AngularAcceleration operator+(const AngularAcceleration& a) const;
  AngularAcceleration& operator=(glm::vec3 v);
  AngularVelocity value;
  Time::Unit timeUnit;
};

Force operator* (const Force& f, const Orientation& o);

#endif