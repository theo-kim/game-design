#ifndef EVILSHIP_DEF
#define EVILSHIP_DEF

#include "standard.h"
#include "Ship.h"

class EvilShip : public Ship {
public:
  EvilShip();
  EvilShip(ShaderProgram *programs, TextureSheet *texture, glm::vec3 pos, QuadTree *engine);
  void Render();
  void AddGun(Gun gun);
  virtual void Update(float delta);
private:
  TextureSheet *texture;
};

#endif
