#include "include/Lander.h"
#define FIRE_SIZE 7.0f;

Lander::Lander () {}

Lander::Lander (ShaderProgram *_program, glm::vec3 initPos, float initSize, const char* texturePath, int _f, float _m, float _t, float _r) {
  program = _program;
  pos = initPos;
  texture = TextureSheet(texturePath, 1, 11);
  float spriteSize = texture.GetSpriteSize();
  size = glm::vec3(spriteSize * initSize, initSize, 1.0f);
  mov = glm::vec3(0.0f, 0.0f, 0.0f);
  thrust = _t;
  mass = _m;
  fuel = _f;
  rotationalVelocity = glm::radians(_r);
  rotation = glm::radians(0.0f);
  rotationDirection = 0.0f;
  animState = 0;
  isAlive = true;
  zoom = glm::vec3(1.0f);
}

void Lander::ZoomTo(float scale) {
  zoom = glm::vec3(scale, scale, 1.0f);
}

void Lander::Render () {
  if (!isAlive) return;
  
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  // Position and size the model matrix
  if (zoom == glm::vec3(1.0f)) {
    modelMatrix = glm::translate(modelMatrix, pos);
  }
  modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
  modelMatrix = glm::scale(modelMatrix, size);
  modelMatrix = glm::scale(modelMatrix, zoom);
  program->SetModelMatrix(modelMatrix);
  
  float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
  float texCoords[12];

  getTexVertices(texCoords);

  glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
  glEnableVertexAttribArray(program->positionAttribute);

  glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
  glEnableVertexAttribArray(program->texCoordAttribute);
  
  glBindTexture(GL_TEXTURE_2D, texture.GetId());
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glDisableVertexAttribArray(program->positionAttribute);
  glDisableVertexAttribArray(program->texCoordAttribute);
}
float Lander::GetRotation() {
  return rotation;
}

glm::mat4x3 Lander::GetBounds () {  
  glm::vec3 topLeft = pos + glm::vec3(-0.5f, 0.5f, 1.0f) * size;
  glm::vec3 bottomLeft = pos + glm::vec3(-0.5f, -0.5f, 1.0f) * size;
  glm::vec3 topRight = pos + glm::vec3(0.5f, 0.5f, 1.0f) * size;
  glm::vec3 bottomRight = pos + glm::vec3(0.5f, -0.5f, 1.0f) * size;

  bottomLeft[1] -= (1.0f / 76.0f) * bottomLeft[1];
  bottomRight[1] = bottomLeft[1];
  
  return glm::mat4x3(topLeft, bottomLeft, topRight, bottomRight);
}

void Lander::getTexVertices(float coords[]) {
  int state = 1;
  if (isThrusting) {
    if ((animState / 50) % 2 == 0) {
      state = 2;
    }
    else {
      state = 0;
    }
    isThrusting = false;
  }
  else if (isExploded) {
    state = 3 + (animState / 10);
    if (state >= 10) {
      isAlive = false;
    }
  }
  texture.GetSprite(coords, state, 0);
}


void Lander::Update (float delta) {
  ++animState;
  if (animState > 1000) animState = 0;
  if (isExploded) return;
  glm::vec3 acc = netForce / mass;
  mov = mov + (acc * delta);
  pos += mov * delta;
  rotation += rotationDirection * (rotationalVelocity * delta);
  netForce = glm::vec3(0.0f);
}

void Lander::ApplyForce (glm::vec3 f) {
  netForce += f;
}

void Lander::StopRotation() {
  rotationDirection = 0.0f;
}

void Lander::RotateCounterclockwise() {
  if (isLanded) return;
  rotationDirection = -1.0f;
}

void Lander::RotateClockwise() {
  if (isLanded) return;
  rotationDirection = 1.0f;
}

void Lander::Thrust() {
  if (isLanded) return;
  float x = glm::sin(rotation) * thrust;
  float y = glm::cos(rotation) * thrust;
  ApplyForce(glm::vec3(-1 * x, y, 0.0f));
  isThrusting = true;
  --fuel;
}

int Lander::GetFuel() {
  return fuel;
}

float Lander::GetMass() {
  return mass;
}

glm::vec3 Lander::GetForce() {
  return netForce;
}

void Lander::Collision(bool good) {
  if (mov[1] < -0.25 || mov[0] > 0.15 || mov[0] < -0.15) Crash();
  else if (!good && !isExploded) Crash();
  else isLanded = true;
  mov = glm::vec3(0.0f);
}

void Lander::Crash() {
  isExploded = true;
  animState = true;
  size *= 2;
}

glm::vec3 Lander::GetPos() {
  return pos;
}
