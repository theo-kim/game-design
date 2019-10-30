#ifndef SHIP_DEF
#define SHIP_DEF

#include "standard.h"
#include "Entity.h"
#include "Sensor.h"
#include "ProximitySensor.h"
#include "EdgeSensor.h"
#include "VertexSensor.h"
#include "Gun.h"
#include "QuadTree.h"

class Gun;
class Bullet;

class Ship : public Collidable {
public:
  Ship();
  Ship(ShaderProgram* programs, QuadTree *collisionEngine, TextureSheet *texture);
  Ship(ShaderProgram* programs, QuadTree *collisionEngine, glm::vec3 p, glm::vec3 s, TextureSheet *texture);
  ~Ship();
  void Render();
  void Update(float delta);
  void Move(bool up, bool down, bool left, bool right);
  void MoveForward(bool go);
  void Fire(int gunIndex);

  // Collision interfaces
  virtual bool DidUpdate();
  virtual void DidCollide(Collidable *with);
  virtual int CheckCollision(Collidable *with);
  int CheckCollision(Bullet *with);
  virtual Collidable::ColliderType GetColliderType();
  bool IsAlive();

  glm::vec3 GetMov() const;
 
  // I'm lazy and putting the sensors in the public space
  Sensor *sensors[9]; // For now...
  ProximitySensor prox;
  EdgeSensor edges[4];
  VertexSensor verts[4];
protected:
  std::vector<Gun> guns;
  float speed;
private:
  int health;
  glm::vec3 mov;
  int isMoving;
};

#endif
