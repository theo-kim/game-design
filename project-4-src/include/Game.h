#ifndef GAME_DEF
#define GAME_DEF

// Game Configuration Settings
#define GAME_NAME "Space War"
#define FIXED_TIMESTEP 0.01666f
#define GRAVITY 0.1f;

#include "standard.h"
#include "Text.h"
#include "Entity.h"
#include "Ship.h"

class Game {
 public:
  Game(float screenHeight, float screenWidth);
  void Initialize();
  void Render();
  void Update();
  void Input();
  void Run();
  void Shutdown();
 private:
  // Physics Methods

  // Physics Variables

  // Camera Methods
  void CameraPan(glm::vec3 pan);
  void CameraZoom(float zoomFactor);
  void CameraRotate(float rotationFactor);

  // Camera Variables
  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;
  
  // Implementation Variables
  ShaderProgram rendererTextured;
  ShaderProgram rendererUntextured;
  SDL_Window* displayWindow;

  // Game state tracker
  int gameState;

  // Frame rate variables
  float lastTicks;
  float accumulator;

  // Window Variables
  float top;
  float left;
  float right;
  float bottom;

  // BEGIN GAME SPECIFIC VARIABLES
  // Entities
  Ship ship;
  
  // Text
};

#endif
