#ifndef GAME_DEF
#define GAME_DEF

// Game Configuration Settings
#define GAME_NAME "Mars Lander"
#define FIXED_TIMESTEP 0.01666f
#define GRAVITY 0.2f;

#include "standard.h"
#include "Lander.h"
#include "Terrain.h"
#include "Text.h"

class Game {
 public:
  Game(float screenHeight, float screenWidth);
  void Initialize();
  void Render();
  void Update();
  void Input();
  void Run();
  void Shutdown();
 
  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;
 private:
  // Physics Methods
  void Gravity(Lander *object);
  int CheckCollision(Lander *lander, Terrain *terrain);

  // Physics Variables
  float g;
  
  // Implementation Variables
  ShaderProgram rendererTextured;
  ShaderProgram rendererUntextured;
  SDL_Window* displayWindow;
  
  int gameState;
  
  float lastTicks;
  float accumulator;
  
  int score;

  Lander lander;
  Terrain land;

  float top;
  float left;
  float right;
  float bottom;

  bool isThrusting; // State whether or not the player is thrusting

  Text fuelCount;
  Text fuelLabel;
  Text endingText;
};

#endif
