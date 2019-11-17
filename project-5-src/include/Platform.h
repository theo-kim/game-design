#ifndef PLATFORM_DEF
#define PLATFORM_DEF

#include "standard.h"
#include "Entity.h"
#include "CollisionEngine.h"

class Platform : virtual public Entity, public Collidable {
public:
    Platform();
    Platform(float width);

    // Entity Implementation
    virtual void Render();
    virtual void Update(float delta);

    // Collidable Implementation
    virtual void DidCollide(Collidable *with);
    virtual int CheckCollision(Collidable *with);
    virtual bool DidUpdate();
    virtual Collidable::ColliderType GetColliderType();
private:
    float width;
    float height;
};

#endif