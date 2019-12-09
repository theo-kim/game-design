#ifndef GRAVITY_DEF
#define GRAVITY_DEF

#include "standard.h"
#include "physics/entity/PhysicsEntity.h"

// Gravitational constant
const Large G = Large(6.7f, -11);

// Gravity is a friend function
void Gravity(PhysicsEntity &p1, PhysicsEntity &p2);

#endif