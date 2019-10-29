#ifndef BULLET_DEF
#define BULLET_DEF

#include "standard.h"
#include "Entity.h"
#include "Ship.h"
#include "QuadTree.h"

class Ship;

class Bullet : public Collidable {
public:
  Bullet();
  Bullet(ShaderProgram *program, QuadTree *collisionEngine, glm::vec3 origin, glm::vec3 c, float s, float p, int pow);
  ~Bullet();
  void Update(float delta);
  void Render();

  void SetOrigin(glm::vec3 _pos, float _rot);
  
  bool IsAlive();
  bool ReadyToDie();

  // Collision interface functions
  virtual bool DidUpdate();
  virtual void DidCollide(Collidable *with);
  virtual int CheckCollision(Collidable *with);
  virtual Collidable::ColliderType GetColliderType();
private:
  float r, g, b;
  glm::vec3 mov;
  float speed;
  float penetration;
  int power;
  bool alive;
  bool killme;
};

#endif
