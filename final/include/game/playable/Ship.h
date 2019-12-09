#ifndef SHIP_DEF
#define SHIP_DEF

#include "standard.h"
#include "physics/entity/PhysicsEntity.h"

class Ship : public PhysicsEntity {
public:
  Ship(Transformation *t);
};

#endif