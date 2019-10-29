#ifndef HERO_DEF
#define HERO_DEF

#include "standard.h"
#include "Ship.h"
#include "Gun.h"

class HeroShip : public Ship {
public:
  HeroShip();
  HeroShip(ShaderProgram **programs, TextureSheet *texture, TextureSheet *gunTex, QuadTree *engine);
  void Render();
  void MoveGun(float radians);
private:
  TextureSheet *texture;
};

#endif
