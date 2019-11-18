#include "../include/framework/engines/PhysicsEngine.h"

//
// BEGIN PHYSICAL ABSTRACT CLASS
Physical::Physical (PhysicsEngine *_engine, float _mass) 
    : engine(_engine),
      mass(_mass) {}

PhysicsEngine *Physical::GetEngine() const {
    return engine;
}

float Physical::GetMass() const {
    return mass;
}
// END PHYSICAL ABSTRACT CLASS
//
// BEGIN PHYSICS ENGINE
PhysicsEngine::PhysicsEngine() {}

PhysicsEngine::PhysicsEngine(float _gravity) 
    : gravity(_gravity) {}

float PhysicsEngine::GetGravity() const {
    return gravity;
}
// END PHYSICS ENGINE
//