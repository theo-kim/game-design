#include "include/HeroShip.h"

HeroShip::HeroShip () {}

HeroShip::HeroShip (ShaderProgram **programs, TextureSheet *texture, TextureSheet *gunTex)
  : Ship(programs[0], texture)
{
  guns.push_back(DoubleGun(programs, this, gunTex));
}

void HeroShip::MoveGun(float radians) {
  guns[0].Rotate(radians);
}

void HeroShip::Render () {
  Ship::Render();
  guns[0].Render();
}
