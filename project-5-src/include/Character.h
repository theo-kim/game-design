#ifndef CHARACTER_DEF
#define CHARACTER_DEF

#include "standard.h"
#include "framework/Entity.h"
#include "framework/engines/standard.h"

class Character : public TexturedEntity, public Collidable, public Physical {
public:
    Character(TexturedShader *_r, TextureSheet *_t, PhysicsEngine *_p, CollisionEngine *_c, float _sp, int _mH, float _m);

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
    virtual glm::vec3 Push() const;
    virtual void PushBy(glm::vec3 force);
    virtual void GravitateTo(glm::vec3 direction, float g);
private:
    float speed;
    int maxHealth;
    int currentHealth;
};

#endif