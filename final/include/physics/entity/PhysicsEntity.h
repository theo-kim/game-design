#ifndef PHYSICS_ENTITY_DEF
#define PHYSICS_ENTITY_DEF

#include "standard.h"
#include "ui/entity/Entity.h"
#include "ui/program/Program.h"
#include "physics/properties/Displacement.h"
#include "physics/properties/Velocity.h"
#include "physics/properties/Acceleration.h"
#include "physics/properties/Angle.h"
#include "physics/properties/Shapes.h"
#include "physics/properties/Length.h"


class PhysicsEntity : public Entity {
public:
  PhysicsEntity(Position p, Orientation o, Shape s, Mass m, Mesh *mesh, ShaderProgram *shader);
  
  virtual void ApplyForce(Force f);
  virtual void ApplyTorque(Torque t);

  virtual void Update(float delta);

  friend void Gravity(PhysicsEntity &p1, PhysicsEntity &p2);
protected:
  // Properties
  Moment moment[3];
  Mass mass;
  Position currentPosition;
  Velocity currentVelocity;
  Acceleration currentAcceleration;

  Orientation currentOrientation;
  AngularVelocity currentAngularVelocity;
  AngularAcceleration currentAngularAcceleration;
};

#endif