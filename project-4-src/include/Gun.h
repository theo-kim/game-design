#ifndef GUN_DEF
#define GUN_DEF

#include "Entity.h"
#include "Ship.h"
#include "VertexSensor.h"
#include "Bullet.h"

class Ship;

class Gun : public Entity {
public:
  Gun();
  Gun(ShaderProgram *program, Ship *_owner, Bullet temp, TextureSheet *texture);
  void Update(float delta);
  void Render();
  void Rotate(float deg);
  void Fire();
protected:
  Entity *owner;
  std::vector<VertexSensor> firePoints;
  int numShots;
  int numShooting;
  Bullet bulletTemplate;
  std::vector<Bullet> activeBullets;
};

class DoubleGun : public Gun {
public:
  DoubleGun();
  DoubleGun(ShaderProgram **program, Ship *_owner, TextureSheet *texture);
};

#endif