#ifndef CHARACTER_DEF
#define CHARACTER_DEF

#include "standard.h"
#include "Entity.h"
#include "CollisionEngine.h"
#include "PhysicsEngine.h"

class Character : virtual public Entity, public Collidable, public Physical {
public:
    Character(ShaderProgram *_r, TextureSheet *_t, PhysicsEngine *_p, CollisionEngine *_c, float _sp, int _mH, float _m);

    float GetSpeed() const;
    int GetHealth() const; 

    // Entity Implementation
    virtual void Render();
    virtual void Update(float delta);

    // Collidable Implementation
    virtual void DidCollide(Collidable *with);
    virtual int CheckCollision(Collidable *with);
    virtual bool DidUpdate();
    virtual Collidable::ColliderType GetColliderType();

    // Physical Implementation
private:
    float speed;
    int maxHealth;
    int currentHealth;
};

#endif