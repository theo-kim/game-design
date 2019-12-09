#ifndef HUMAN_DEF
#define HUMAN_DEF

#include "standard.h"
#include "Controller.h"
#include "game/playable/Ship.h"

class Human : public Controller {
public:
  Human(Ship* s); // Humans can only control ships
  virtual void Input(); // Human input comes from SDL events
  virtual void Update(float delta); // Humans don't require feedback
};

#endif