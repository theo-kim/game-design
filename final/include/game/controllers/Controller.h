#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "standard.h"
#include "game/playable/ControllableEntity.h"

class ControllableEntity;

// This class controls the behaviors of an entity
// Used for AIs, human players, and networked behaviors
// Controllers is an abstract class and is friend to 
// Controllable Entity
class Controller {
public:
  class State {
  public:
    State() : currentState(0) {}
    State(int i) : currentState(i) {}
    bool operator==(const State& s) const { return currentState == s.currentState; }
    int currentState;
  };

  const static State NoState;

  Controller(ControllableEntity* e) : entity(e) {}
  // Abstracted method for the controller input
  virtual void Input(SDL_Event& e) = 0;
  // Abstracted method for the controller state update
  virtual void Update(float delta) = 0;

  virtual State GetState() const { return currentState; }
  virtual void ClearState() {}
protected:
  ControllableEntity* entity;
  State currentState;
};

#endif