#ifndef CHARACTER_DEF
#define CHARACTER_DEF

#include "standard.h"
#include "framework/Entity.h"
#include "framework/engines/standard.h"
#include "framework/sensors/standard.h"

class Character : public TexturedEntity, public Collidable, public Physical {
public:
    Character(TexturedShader *_r, TextureSheet *_t, PhysicsEngine *_p, CollisionEngine *_c, float _sp, int _mH, float _m);

    float GetSpeed() const;
    int GetHealth() const; 

    void Walk(int dir);
    void Jump(bool doit);
    void Stop();
    virtual void Ult(bool doit);

    // Entity Implementation
    virtual void Render();
    virtual void Update(float delta);
    virtual bool GetGarbage();

    // Collidable Implementation
    virtual void DidCollide(Collidable *with);
    virtual int CheckCollision(Collidable *with);
    virtual bool DidUpdate();
    virtual Collidable::ColliderType GetColliderType();

    // Physical Implementation
    virtual glm::vec3 Push() const;
    virtual void PushBy(glm::vec3 force);
    virtual void GravitateTo(glm::vec3 direction, float g);

    bool ulting;
    float threshold;
    ProximitySensor p;
    std::vector<EdgeSensor> edges;
    glm::vec3 scale;
    int currentHealth;
    glm::vec3 mov;
protected:
    int walking;
    int jumping;
    int animState;
    float accumulator;
private:
    float speed;
    int maxHealth;
};

#endif