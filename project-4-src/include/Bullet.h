#ifndef BULLET_DEF
#define BULLET_DEF

#include "standard.h"
#include "Entity.h"

class Bullet : public Entity {
public:
  Bullet();
  Bullet(ShaderProgram* program, glm::vec3 origin, glm::vec3 c, float s, float p, int pow);
  void Update(float delta);
  void Render();

  void SetOrigin(glm::vec3 _pos, float _rot);
  
  bool IsAlive();
private:
  float r, g, b;
  glm::vec3 mov;
  float speed;
  float penetration;
  int power;
  bool alive;
};

#endif
