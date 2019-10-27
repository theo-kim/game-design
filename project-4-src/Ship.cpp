#include "include/Ship.h"

Ship::Ship() {}

Ship::Ship(ShaderProgram *_program, TextureSheet *texture)
  : Entity(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, texture, _program) {
  mov = glm::vec3(0.0f);
  speed = 1;
}

Ship::Ship(ShaderProgram *_program, glm::vec3 p, glm::vec3 s, TextureSheet *texture)
  : Entity(p, s, 0.0f, texture, _program) {
  mov = glm::vec3(0.0f);
  speed = 1;
}

Ship::~Ship() {
  for (int i = 0; i < guns.size(); ++i) {
    //delete guns[i];
  }
}

void Ship::Update(float delta) {
  pos += mov * delta * speed;
  for (int i = 0; i < guns.size(); ++i) {
    guns[i].Update(delta);
  }
  return;
}

void Ship::Render() {
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, pos);
  modelMatrix = glm::scale(modelMatrix, size);
  
  float map[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5,
		  -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1 };

  // for (int i = 0; i < 4; ++i) {}
  
  texture->GetSprite(map + 12, 0, 0);

  program->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
  
  Entity::Render(modelMatrix, texture->GetId(), map, 12);
}

void Ship::Move(bool up, bool down, bool left, bool right) {
  isMoving = 0 & (up << 3) & (down << 2) & (left << 1) & right;
  mov = glm::vec3((float)right - (float)left, (float)up - (float)down, 0.0f);
}

glm::vec3 Ship::GetMov() const {
  return mov * speed;
}

void Ship::Fire(int gunIndex) {
  guns[gunIndex].Fire();
}
