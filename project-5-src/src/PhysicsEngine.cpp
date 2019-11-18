#include "../include/framework/engines/PhysicsEngine.h"

//
// BEGIN PHYSICAL ABSTRACT CLASS
Physical::Physical (PhysicsEngine *_engine, float _mass) 
    : engine(_engine),
      mass(_mass),
      acceleration(glm::vec3(0.0f)),
      netForce(glm::vec3(0.0f)) {}
      

void Physical::Update(float delta) {
    GetPhysicsEngine()->RegisterEntity(this);
}

PhysicsEngine *Physical::GetPhysicsEngine() const {
    return engine;
}

void Physical::SetForce(glm::vec3 f) {
    netForce = f;
}

void Physical::SetAcceleration(glm::vec3 a) {
    acceleration = a;
}

void Physical::AddForce(glm::vec3 f) {
    netForce += f;
}

float Physical::GetMass() const {
    return mass;
}

glm::vec3 Physical::GetForce() const {
    return netForce;
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

void PhysicsEngine::RegisterEntity(Physical* physical) {
    physical->GravitateTo(glm::vec3(0.0f, -1.0f, 0.0f), gravity);
    // physical->PushBy(physical->GetForce());
}
// END PHYSICS ENGINE
//