#include "include/Pong.h"
#include <string>

Pong::Pong (float screenHeight, float screenWidth) : top((screenHeight / screenWidth) * 5.0f),
						    bottom(-1 * top),
						    left(-5.0f),
						    right(5.0f) {

  player1 = Paddle(&programUntextured, left + 0.25, 0, 1, 0.2, 5);
  player2 = Paddle(&programUntextured, right - 0.25, 0, 1, 0.2, 5);
  topWall = Paddle(&programUntextured, 0, top - 0.65, 0.3, right - left, 0);
  bottomWall = Paddle(&programUntextured, 0, bottom + 0.15, 0.3, right - left, 0);
  player1Lose = Paddle(&programUntextured, left - 0.1, 0, top - bottom, 0.2, 0);
  player2Lose = Paddle(&programUntextured, right + 0.1, 0, top - bottom, 0.2, 0);

  ball = Paddle(&programUntextured, 0, 0, 0.2, 0.2, 2);
  ball.MoveFree(glm::vec3(1.0f, 1.0f, 0.0f));
  
  viewMatrix = glm::mat4(1.0f);
  projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

  player1Moving = 0;
  player2Moving = 0;

  player1Score = 0;
  player2Score = 0;
}

void Pong::Render () {
  player1.Render();
  player2.Render();
  topWall.Render();
  bottomWall.Render();
  ball.Render();
  player1ScoreText.Render();
  player2ScoreText.Render();
  titleText.Render();
}

void Pong::InitText (GLuint texture) {
  player1ScoreText = Text(std::to_string(player1Score), &programTextured, left + 1, top - 0.23, 0.2, 0.4, texture);
  player2ScoreText = Text(std::to_string(player2Score), &programTextured, right - 1, top - 0.23, 0.2, 0.4, texture);
  titleText = Text("Pong", &programTextured, 0, top - 0.23, 0.2, 0.4, texture);
}

void Pong::Update (float delta) {
  player1.Update(delta);
  player2.Update(delta);
  ball.Update(delta);

  if (CheckPaddleCollision(&player1, &topWall)) {
    player1.Hold('t', topWall.GetBounds()[1]);
  }
  else if (CheckPaddleCollision(&player1, &bottomWall)) {
    player1.Hold('b', bottomWall.GetBounds()[0]);
  }

  if (CheckPaddleCollision(&player2, &topWall)) {
    player2.Hold('t', topWall.GetBounds()[1]);
  }
  else if (CheckPaddleCollision(&player2, &bottomWall)) {
    player2.Hold('b', bottomWall.GetBounds()[0]);
  }

  if (CheckBallCollision(&topWall) || CheckBallCollision(&bottomWall)) {
    ball.mov[1] *= -1;
    ball.pos[1] *= 0.99;
  }

  if (CheckBallCollision(&player1) || CheckBallCollision(&player2)) {
    ball.mov[0] *= -1;
    ball.speed += 0.25;
  }

  if (CheckBallCollision(&player1Lose)) {
    ball.pos = glm::vec3(0.0f);
    ball.speed = 2;
    ++player2Score;
    player2ScoreText.SetText(std::to_string(player2Score));
    if (player2Score > WINNING) {
      titleText.SetText("Player 2 Wins!");
      ball.speed = 0;
    }
  }
  else if (CheckBallCollision(&player2Lose)) {
    ball.pos = glm::vec3(0.0f);
    ball.speed = 2;
    ++player1Score;
    player1ScoreText.SetText(std::to_string(player1Score));
    if (player1Score > WINNING) {
      titleText.SetText("Player 1 Wins!");
      ball.speed = 0;
    }
  }
}

bool Pong::CheckBallCollision (Paddle *paddle) {
  return CheckPaddleCollision (&ball, paddle);
}
   
bool Pong::CheckPaddleCollision (Paddle *a, Paddle *b) {
  glm::mat4x3 aBounds = a->GetBounds();
  glm::mat4x3 bBounds = b->GetBounds();

  if (aBounds[0][0] > bBounds[3][0] || bBounds[0][0] > aBounds[3][0]) {
    return false;
  }
  if (aBounds[0][1] < bBounds[3][1] || bBounds[0][1] < aBounds[3][1]) {
    return false;
  }

  return true;
};

void Pong::Input (SDL_Event e) {
  const Uint8 *keys = SDL_GetKeyboardState(NULL);
  
  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
    case SDLK_UP :
      player2.Move(1.0f);
      ++player2Moving;
      break;
    case SDLK_DOWN :
      player2.Move(-1.0f);
      ++player2Moving;
      break;
    case SDLK_w :
      player1.Move(1.0f);
      ++player1Moving;
      break;
    case SDLK_s :
      player1.Move(-1.0f);
      ++player1Moving;
      break;
    default:
      break;
    }
  }
  else if (e.type == SDL_KEYUP) {
    switch (e.key.keysym.sym) {
    case SDLK_UP :
    case SDLK_DOWN :
      --player2Moving;
      break;
    case SDLK_w :
    case SDLK_s :
      --player1Moving;
      break;
    default:
      break;
    }

    if (player1Moving == 0 || (!keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_S])) {
      player1.Move(0.0f);
      player1Moving = 0;
    }
    if (player2Moving == 0  || (!keys[SDL_SCANCODE_UP] && !keys[SDL_SCANCODE_DOWN])) {
      player2.Move(0.0f);
      player2Moving = 0;
    }
  }
}
