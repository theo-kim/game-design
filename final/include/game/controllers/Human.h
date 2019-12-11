#ifndef HUMAN_DEF
#define HUMAN_DEF

#include "standard.h"
#include "Controller.h"
#include "game/playable/Ship.h"
#include <map>

class Ship;

class Human : public Controller {
public:
  Human(Ship* s); // Humans can only control ships
  virtual void Input(SDL_Event& e); // Human input comes from SDL events
  virtual void Update(float delta); // Humans don't require feedback
private:
  Ship *ship;
  std::map<int, bool> keys;
};

#endif