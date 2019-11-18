#include "../include/framework/animation/standard.h"

//
// BEGIN ANIMATION DECLARATION :
Animation::Animation(Entity *_target) 
    : EntityGroup({ _target }) {}

// Getter
float Animation::GetAccumulator() const {
    return accumulator;
}

Entity *Animation::GetEntity() const {
    return EntityGroup::GetEntity(0);
}

// Setter
void Animation::ClearAccumulator() {
    accumulator = 0;
}
void Animation::SetEntity(Entity *e) {
    Empty();
    EntityGroup::AddEntity(e);
}

// Adder
void Animation::AddAccumulator(float addition) {
    accumulator += addition;
}
// : END ANIMATION DECLARATION
// 
// 
// BEGIN BLINK DECLARATION :
Blink::Blink(Entity *_target, float _interval) 
    : Animation(_target),
      interval(_interval),
      blink(false),
      doit(true) {
          ClearAccumulator();
      }

Blink::Blink(Entity *_target, float _interval, bool _auto) 
    : Animation(_target),
      interval(_interval),
      blink(false),
      doit(_auto) {
          ClearAccumulator();
      }

void Blink::Forward() {
    doit = false;
    blink = true;
};
void Blink::Reverse() {
    doit = false;
    blink = false;
};
void Blink::StopAndReverse() {
    Reverse();
};

void Blink::Render() {
    GetEntity()->Render();
}

void Blink::Update(float delta) {
    GetEntity()->Update(delta);

    GetEntity()->SetRenderFlag(!blink);
    if (!doit) return;
    AddAccumulator(delta);
    if (GetAccumulator() >= interval) {
        blink = !blink;
        // ClearAccumulator();
        AddAccumulator(-interval);
    }
}
//
//
//
//
Slide::Slide(Entity *_target, float _duration, glm::vec3 _distance) 
    : Animation(_target),
      duration(_duration),
      distance(_distance),
      doingit(false),
      doit(false),
      reversing(false),
      done(false)
{}

Slide::Slide(Entity *_target, float _duration, glm::vec3 _distance, bool _doit) 
    : Animation(_target),
      duration(_duration),
      distance(_distance),
      doingit(false),
      doit(_doit),
      reversing(false),
      done(false)
{}

void Slide::Forward() {
    doit = true;
    doingit = false;
    reversing = false;
}
void Slide::Reverse() {
    reversing = true;
    doit = true;
    done = false;
    increment = (-distance) * (1 / duration);
    ClearAccumulator();
    AddAccumulator(duration);
    doingit = true;
}
void Slide::StopAndReverse() {
    reversing = true;
    doit = true;
    done = false;
    float current = GetAccumulator();

    if (current == 0) {
        Reverse();
    }
    else {
        float diff = duration - current;
        increment = (-distance) * (1 / duration);
        ClearAccumulator();
        AddAccumulator(diff);
        doingit = true;
    }
}

void Slide::Render() {
    GetEntity()->Render();
}

void Slide::Update(float delta) {
    GetEntity()->Update(delta);

    if (done) return;
    if (!doit) return;
    if (!doingit) {
        increment = distance * (1 / duration);
        if (GetAccumulator() == 0) {
            ClearAccumulator();
            AddAccumulator(duration);
        }
        else {
            float diff = duration - GetAccumulator();
            ClearAccumulator();
            AddAccumulator(diff);
        }
        doingit = true;
    }
    float amount = 0.0f;
    if (delta < GetAccumulator()) {
        amount = delta;
        AddAccumulator(-delta);
    }
    else {
        amount = GetAccumulator();
        ClearAccumulator();
        doit = false;
        doingit = false;
        if (reversing) {
            done = false;
            reversing = false;
        }
        else {
            done = true;
        }
    }
    GetEntity()->AddPos(increment * amount);
}

