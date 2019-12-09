#ifndef NETWORK_DEF
#define NETWORK_DEF

#include "standard.h"
#include "Controller.h"
#include "game/playable/ControllableEntity.h"
#include "NetworkConnection.h"

class Network : public Controller {
public:
  Network(ControllableEntity *e, NetworkConnection *n);
  virtual void Input(); // read from server state and update entity
  virtual void Update(float delta); // write to server current entity state
private:
  NetworkConnection *conn;
  // Define a state
};

#endif