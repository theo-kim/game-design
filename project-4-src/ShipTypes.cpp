#include "include/HeroShip.h"
#include "include/EvilShip.h"

HeroShip::HeroShip () {}

HeroShip::HeroShip (ShaderProgram **programs, TextureSheet *texture, QuadTree *engine)
  : Ship(programs[0], engine, texture)
{
  guns.push_back(DoubleGun(programs, this, engine));
}

void HeroShip::MoveGun(float radians) {
  guns[0].Rotate(radians);
}

void HeroShip::Render () {
  Ship::Render();
  guns[0].Render();
}

void HeroShip::DidCollide(Collidable *with) {
  Ship::DidCollide(with);
}

EvilShip::EvilShip () {}

EvilShip::EvilShip(ShaderProgram *program, TextureSheet *texture, glm::vec3 _pos, QuadTree *engine)
  : Ship(program, engine, _pos, glm::vec3(0.7f, 0.7f, 1.0f), texture),
    target(pos)
{
  speed = 0.5;
}

void EvilShip::Render() {
  Ship::Render();
}

void EvilShip::Update (float delta) {
  if (pos[0] < target[0] + 0.1 && pos[0] > target[0] - 0.1 && pos[1] < target[1] + 0.1 && pos[1] > target[1] - 0.1) {
    Retarget(10, 10, 10, 10);
  }
  else if (rand() % 100 < 1) {
    Fire(rand() % guns.size());
    Retarget(10, 10, 10, 10);
  }
  else {
    MoveForward(1);
    float targetPolar = glm::atan(target[1] - pos[1], target[0] - pos[0]);
    rot -= (rot + radians(90) - targetPolar) * delta;
  }
  Ship::Update(delta);
}

void EvilShip::DidCollide(Collidable *with) {
  if (with->GetColliderType() == Collidable::OBJECT) {
    HeroShip *ship = dynamic_cast<HeroShip *>(with);
    if (ship != NULL) {
      // Run away from the hero ship!
      glm::vec3 retreat = GetPos() - ship->GetPos();
      retreat = retreat * 3.0f;
      float up = 0, down = 0, left = 0, right = 0;
      if (retreat[0] < 0) left = std::fabs(retreat[0]);
      else right = retreat[0];
      if (retreat[1] < 0) down = std::fabs(retreat[1]);
      else up = retreat[1];
      Retarget(up, down, left, right);
      speed = 2.0f;
      return; 
    }
  }
  Ship::DidCollide(with);
}

void EvilShip::AddGun(Gun gun) {
  guns.push_back(Gun(gun));
}


void EvilShip::Retarget(float up, float down, float left, float right) {
  down *= -1;
  left *= -1;
  int xRange = (pos[0] + right + std::fabs(left)) * 200;
  int yRange = (pos[1] + up + std::fabs(down)) * 200;

  if (xRange == 0) xRange = 1;
  if (yRange == 0) yRange = 1;
  
  float xRandom = ((float)(rand() % xRange + (left * 100)) / 100);
  float yRandom = ((float)(rand() % yRange + (down * 100)) / 100);
  target = glm::vec3(xRandom, yRandom, 1.0f);

  speed = 0.5f;
}
