#include "include/Ship.h"

Ship::Ship() {}

Ship::Ship(ShaderProgram *_program)
  : Entity(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, _program) {
  mov = glm::vec3(0.0f);
  speed = 1;
}

Ship::Ship(ShaderProgram *_program, glm::vec3 p, glm::vec3 s)
  : Entity(p, s, 0.0f, _program) {
  mov = glm::vec3(0.0f);
  speed = 1;
}

void Ship::Update(float delta) {
  pos += mov * delta * speed;
  return;
}

void Ship::Render() {
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, pos);
  modelMatrix = glm::scale(modelMatrix, size);
  
  float map[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5,
		  -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1 };

  program->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
  
  Entity::Render(modelMatrix, (GLuint)NULL, map, 12);
}

void Ship::Move(bool up, bool down, bool left, bool right) {
  mov = glm::vec3((float)right - (float)left, (float)up - (float)down, 0.0f);
}

glm::vec3 Ship::GetMov() const {
  return mov * speed;
}
