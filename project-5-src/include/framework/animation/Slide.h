#ifndef SLIDE_ANIM_DEF
#define SLIDE_ANIM_DEF

#include "../gl.h"
#include "../Entity.h"
#include "Animation.h"

class Slide : public Animation {
public:
    Slide(Entity *_target, float _duration, glm::vec3 _distance);
    Slide(Entity *_target, float _duration, glm::vec3 _distance, bool _doit);

    // Abstract Implementations
    virtual void Render();
    virtual void Update(float delta);

    virtual void Forward();
    virtual void Reverse();
    virtual void StopAndReverse();
private:
    float duration;
    glm::vec3 increment;
    glm::vec3 distance;
    bool doit;
    bool doingit;
    bool done;
    bool reversing;
};

#endif