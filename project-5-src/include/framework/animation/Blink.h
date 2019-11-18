#ifndef BLINK_DEF
#define BLINK_DEF

#include "../gl.h"
#include "../Entity.h"
#include "Animation.h"

class Blink : public Animation {
public:
    Blink(Entity *_target, float _interval);
    Blink(Entity *_target, float _interval, bool _auto);

    virtual void Forward();
    virtual void Reverse();
    virtual void StopAndReverse();

    // Abstract Implementations
    virtual void Render();
    virtual void Update(float delta);
private:
    bool doit;
    float interval;
    bool blink;
};

#endif