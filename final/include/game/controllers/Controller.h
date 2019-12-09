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
  Controller(ControllableEntity* e) : entity(e) {}
  // Abstracted method for the controller input
  virtual void Input() = 0;
  // Abstracted method for the controller state update
  virtual void Update(float delta) = 0;
protected:
  ControllableEntity* entity;
};

#endif