#ifndef SHIP_DEF
#define SHIP_DEF

#include "standard.h"
#include "Entity.h"

class Ship : public Entity {
 public:
  Ship();
  Ship(ShaderProgram* program);
  void Render();
  void Update();
 private:
  int health;
};

#endif
