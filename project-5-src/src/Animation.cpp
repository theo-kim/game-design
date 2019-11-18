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
      blink(false) {
          ClearAccumulator();
      }

void Blink::Render() {
    if (!blink) GetEntity()->Render();
}

void Blink::Update(float delta) {
    AddAccumulator(delta);
    while (GetAccumulator() >= interval) {
        blink = !blink;
        // ClearAccumulator();
        AddAccumulator(-interval);
    }
}

