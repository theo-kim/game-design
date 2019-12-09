#ifndef CONTROLLABLE_DEF
#define CONTROLLABLE_DEF

#include "standard.h"
#include "physics/entity/PhysicsEntity.h"
#include "game/controllers/Controller.h"

class Controller;

class ControllableEntity {
public:
  ControllableEntity(/* args */);
  ~ControllableEntity();

  friend class Controller; // Controllers need access to everything
private:
  Controller *c;
};

#endif