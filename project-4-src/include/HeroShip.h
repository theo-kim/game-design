#ifndef HERO_DEF
#define HERO_DEF

#include "standard.h"
#include "Ship.h"
#include "Gun.h"

class HeroShip : public Ship {
public:
  HeroShip();
  HeroShip(ShaderProgram **programs, TextureSheet *texture, QuadTree *engine);
  void Render();
  void MoveGun(float radians);
  virtual void DidCollide(Collidable *with);
private:
  TextureSheet *texture;
};

#endif
