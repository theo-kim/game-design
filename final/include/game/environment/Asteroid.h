#ifndef ASTEROID_DEF
#define ASTEROID_DEF

#include "standard.h"
#include "physics/entity/PhysicsEntity.h"
#include "game/controllers/Collision.h"

class Asteroid : public PhysicsEntity {
public:
  Asteroid(Position p, Mass m, Length r, AngularVelocity a, Velocity v, Mesh *mesh, ShaderProgram *sp);
  void Render();
  friend class CollisionController;
};

#endif