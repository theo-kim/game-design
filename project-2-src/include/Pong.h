#ifndef PONG_DEF
#define PONG_DEF

#include "standard.h"
#include "Paddle.h"

class Pong {
 public:
  Pong(float screenHeight, float screenWidth);
  void Render();
  void Update(float delta);
  void Input(SDL_Event e);
  
  ShaderProgram programTextured;
  ShaderProgram programUntextured;
  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;
 private:
  bool CheckPaddleCollision (Paddle *a, Paddle *b);
  bool CheckBallCollision (Paddle *a);
  
  int player1Moving;
  int player2Moving;
  
  int player1Score;
  int player2Score;

  Paddle player1;
  Paddle player2;
  Paddle topWall;
  Paddle bottomWall;
  Paddle player1Lose;
  Paddle player2Lose;

  Paddle ball;

  float top;
  float left;
  float right;
  float bottom;
};

#endif
