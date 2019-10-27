#include "include/Bullet.h"

Bullet::Bullet() {}

Bullet::Bullet(ShaderProgram *program, glm::vec3 origin, glm::vec3 c, float s, float p, int pow)
  : Entity(origin, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, program),
    power(pow),
    speed(s),
    penetration(p),
    mov(glm::vec3(1.0f, 0.0f, 0.0f)),
    r(c[0]),
    g(c[1]),
    b(c[2]),
    alive(true)
{
  
}

void Bullet::Update(float delta) {
  pos += mov * speed * delta;
  penetration -= 0.5;
  if (penetration < 0) {
    alive = false;
  }
}

void Bullet::Render() {
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, pos);
  modelMatrix = glm::rotate(modelMatrix, rot, glm::vec3(0.0f, 0.0f, 1.0f));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.05f, 1.0f));

  program->SetColor(r, g, b, 1.0f);

  float map[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5,
                  -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1 };
  
  Entity::Render(modelMatrix, (GLuint)NULL, map, 12);
}

void Bullet::SetOrigin(glm::vec3 _pos, float _rot) {
  mov = glm::vec3(glm::cos(_rot), glm::sin(_rot), 0.0f);
  rot = _rot;
  pos = _pos;
}

bool Bullet::IsAlive() {
  return alive;
}
