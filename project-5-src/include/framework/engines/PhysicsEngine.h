#ifndef PHYSICS_DEF
#define PHYSICS_DEF

#include "../gl.h"
#include "../Entity.h"

class Physical;
class PhysicsEngine;

class Physical : virtual public Entity {
public:
    Physical(PhysicsEngine *engine, float mass);

    // Interface functions
    virtual glm::vec3 Push() const = 0;
    virtual void PushBy(glm::vec3 force) = 0;
    virtual void GravitateTo(glm::vec3 direction, float g) = 0;

    virtual void Update(float delta);

    // Setters
    void SetForce(glm::vec3 f);
    void SetAcceleration(glm::vec3 a);

    // Getters
    PhysicsEngine *GetPhysicsEngine() const;
    float GetMass() const;
    glm::vec3 GetForce() const;
    glm::vec3 GetAcceleration() const;

    // Adders
    void AddForce(glm::vec3 f);
private:
    PhysicsEngine *engine;
    float mass;
    glm::vec3 netForce;
    glm::vec3 acceleration;
};

class PhysicsEngine {
public:
    PhysicsEngine();
    PhysicsEngine(float _gravity);

    float GetGravity() const;

    void RegisterEntity(Physical* physical);
   
private:
    float gravity;
};

#endif