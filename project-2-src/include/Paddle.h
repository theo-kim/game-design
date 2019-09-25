#ifndef PADDLE_DEF
#define PADDLE_DEF

#include "standard.h"

class Paddle {
 public:
  Paddle();
  Paddle(ShaderProgram *_program, float x, float y, float _height, float _width, float _speed);
  void Render ();
  void Update (float delta);
  void Move (float direction);
  void MoveFree (glm::vec3 mov);
  void Hold (char edge, glm::vec2 boundary);
  glm::mat4x3 GetBounds ();

  float speed;
  glm::vec3 mov;
  glm::vec3 pos;
 private:
  ShaderProgram *program;
  float height;
  float width;
};

#endif
