#include "include/Ship.h"

Ship::Ship() {}

Ship::Ship(ShaderProgram *_program)
  : Entity(glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, _program) {}

void Ship::Update() {
  return;
}

void Ship::Render() {
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  
  float map[] = { -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1,
		  -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1 };

  program->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
  
  Entity::Render(modelMatrix, (GLuint)NULL, map, 12);
}
