#ifndef PONG_DEF
#define PONG_DEF

#define WINNING 4 // The winning score

#include "standard.h"
#include "Paddle.h"
#include "Text.h"

class Pong {
 public:
  Pong(float screenHeight, float screenWidth);
  void InitText(GLuint texture);
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

  Text player1ScoreText;
  Text player2ScoreText;
  Text titleText;
};

#endif
