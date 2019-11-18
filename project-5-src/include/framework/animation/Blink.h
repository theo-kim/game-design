#ifndef BLINK_DEF
#define BLINK_DEF

#include "../gl.h"
#include "../Entity.h"
#include "Animation.h"

class Blink : public Animation {
public:
    Blink(Entity *_target, float _interval);

    // Abstract Implementations
    virtual void Render();
    virtual void Update(float delta);
private:
    float interval;
    bool blink;
};

#endif