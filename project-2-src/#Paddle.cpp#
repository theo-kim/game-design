#include "include/Paddle.h"

Paddle::Paddle () {}

Paddle::Paddle (ShaderProgram *_program, float x, float y, float _height, float _width, float _speed) {
  program = _program;
  pos = glm::vec3(x, y, 0);
  height = _height;
  width = _width;
  speed = _speed;
  mov = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Paddle::Render () {
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  // Position the model matrix
  modelMatrix = glm::translate(modelMatrix, pos);
  modelMatrix = glm::scale(modelMatrix, glm::vec3(width, height, 1.0f));

  float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

  glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
  glEnableVertexAttribArray(program->positionAttribute);
  
  glBindTexture(GL_TEXTURE_2D, (GLuint)NULL);
  program->SetModelMatrix(modelMatrix);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glDisableVertexAttribArray(program->positionAttribute);
}

glm::mat4x3 Paddle::GetBounds () {
  glm::vec3 topLeft = pos + glm::vec3(-width / 2, height / 2, 0);
  glm::vec3 bottomLeft = pos + glm::vec3(-width / 2, -height / 2, 0);
  glm::vec3 topRight = pos + glm::vec3(width / 2, height / 2, 0);
  glm::vec3 bottomRight = pos + glm::vec3(width / 2, -height / 2, 0);
  
  return glm::mat4x3(topLeft, bottomLeft, topRight, bottomRight);
}

void Paddle::Hold (char edge, glm::vec2 boundary) {
  switch (edge) {
  case 'b' :
    pos[1] = boundary[1] + (height / 2);
    break;
  case 't':
    pos[1] = boundary[1] - (height / 2);
    break;
  case 'l':
    pos[0] = boundary[0] + (width / 2);
    break;
  case 'r':
    pos[0] = boundary[0] - (width / 2);
    break;
  }
}

void Paddle::Move (float direction) {
  mov[1] = direction;
}

void Paddle::MoveFree (glm::vec3 direction) {
  mov = direction;
}

void Paddle::Update (float delta) {
  pos += mov * speed * delta;
}
