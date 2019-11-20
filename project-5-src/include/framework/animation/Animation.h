#ifndef ANIM_DEF
#define ANIM_DEF

#include "../gl.h"
#include "../Entity.h"

class Animation : public EntityGroup<Entity> {
public:
    Animation(Entity *_target);

    virtual void Forward() = 0;
    virtual void Reverse() = 0;
    virtual void StopAndReverse() = 0;

    // Getter
    float GetAccumulator() const;
    virtual Entity *GetEntity() const;

    // Setter
    void SetEntity(Entity *e);
    void ClearAccumulator();

    // Adder
    void AddAccumulator(float addition);
protected:
     bool didRender;
private:
    float accumulator;
};

#endif