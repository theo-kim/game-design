#include "include/HeroShip.h"
#include "include/EvilShip.h"

HeroShip::HeroShip () {}

HeroShip::HeroShip (ShaderProgram **programs, TextureSheet *texture, TextureSheet *gunTex, QuadTree *engine)
  : Ship(programs[0], engine, texture)
{
  guns.push_back(DoubleGun(programs, this, gunTex, engine));
}

void HeroShip::MoveGun(float radians) {
  guns[0].Rotate(radians);
}

void HeroShip::Render () {
  Ship::Render();
  guns[0].Render();
}

EvilShip::EvilShip () {}

EvilShip::EvilShip(ShaderProgram *program, TextureSheet *texture, glm::vec3 _pos, QuadTree *engine)
  : Ship(program, engine, _pos, glm::vec3(0.7f, 0.7f, 1.0f), texture)
{
}

void EvilShip::Render() {
  Ship::Render();

  for (int i = 0; i < guns.size(); ++i) {
    guns[i].Render();
  }
}

void EvilShip::Update (float delta) {
  Ship::Update(delta);
}

void EvilShip::AddGun(Gun gun) {
  guns.push_back(Gun(gun));
}
