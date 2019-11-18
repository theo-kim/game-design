#ifndef GAME_DEF
#define GAME_DEF

// Game Configuration Settings
#define GAME_NAME "The Office: A Workplace Platformer"
#define FIXED_TIMESTEP 0.01666f
#define NUM_STARS 300
#define NUM_ENEMIES 3

#include <map>

#include "standard.h"
#include "framework/standard.h"

class Game {
 public:
  Game(float screenHeight, float screenWidth);
  void Initialize();
  void Run();
  void Shutdown();
 private:
  // These really don't need to be shown 
  void Render();
  void Update();
  void Input();

  // For manipulating scenes
  Scene *currentScene;

  // Physics Methods

  // Physics Variables

  // Camera Variables
  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;
  glm::mat4 backgroundProjectionMatrix;
  glm::mat4 overlayProjectionMatrix;
  
  // Implementation Variables
  SDL_Window* displayWindow;

  // Game state tracker
  int gameState;
  bool isPaused;

  // Frame rate variables
  float lastTicks;
  float accumulator;

  // Window Variables
  float width;
  float height;

  float top;
  float left;
  float right;
  float bottom;

  // Scenes
  Scene *opening;
};

#endif
