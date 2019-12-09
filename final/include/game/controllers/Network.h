#ifndef NETWORK_DEF
#define NETWORK_DEF

#include "standard.h"
#include "Controller.h"
#include "game/playable/ControllableEntity.h"

class Network : public Controller {
public:
  Network(ControllableEntity *e);
  virtual void Input(); // read from server state and update entity
  virtual void Update(float delta); // write to server current entity state
private:
  // Define a state
};

#endif