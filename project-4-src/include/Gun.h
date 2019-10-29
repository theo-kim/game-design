#ifndef GUN_DEF
#define GUN_DEF

#include "Entity.h"
#include "Ship.h"
#include "VertexSensor.h"
#include "Bullet.h"
#include "QuadTree.h"

class Ship;
class Bullet;

class Gun : public Entity {
public:
  Gun();
  Gun(ShaderProgram *program, Ship *_owner, Bullet temp, TextureSheet *texture);
  Gun(ShaderProgram *program, Ship *_owner, Bullet temp, TextureSheet *texture, glm::vec3 pos, glm::vec3 size);
  ~Gun();
  void Update(float delta);
  void Render();
  void Rotate(float deg);
  void Fire();
  Entity *owner;
protected:
  std::vector<VertexSensor> firePoints;
  int numShots;
  int numShooting;
  std::vector<Bullet> bulletTemplate;
  std::vector<Bullet *> activeBullets;
};

class DoubleGun : public Gun {
public:
  DoubleGun();
  DoubleGun(ShaderProgram **program, Ship *_owner, TextureSheet *texture, QuadTree *engine);
};

#endif
