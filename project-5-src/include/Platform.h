#ifndef PLATFORM_DEF
#define PLATFORM_DEF

#include "standard.h"
#include "framework/Entity.h"
#include "framework/engines/standard.h"
#include "framework/sensors/standard.h"

class Platform : virtual public UntexturedEntity, public Collidable {
public:
    Platform(UntexturedShader *shader, CollisionEngine *e, glm::vec3 c);
    Platform(UntexturedShader *shader, CollisionEngine *e, glm::vec3 c, glm::vec3 pos, float width);

    // Entity Implementation
    virtual void Render();
    virtual void Update(float delta);

    // Collidable Implementation
    virtual void DidCollide(Collidable *with);
    virtual int CheckCollision(Collidable *with);
    virtual bool DidUpdate();
    virtual Collidable::ColliderType GetColliderType();

    ProximitySensor p;
    std::vector<EdgeSensor> edges;
private:
    float width;
    float height;
};

#endif