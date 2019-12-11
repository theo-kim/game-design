#ifndef GAME_DEF
#define GAME_DEF

// Game Configuration Settings
#define GAME_NAME "The Spawn of Ouranos"
#define FIXED_TIMESTEP 0.006f

#include "./standard.h"
#include "ui/entity/Entity.h"
#include "ui/scene/Scene.h"

class Game {
public:
  Game(float screenHeight, float screenWidth);
  void Initialize();
  void Run();
  void Shutdown();
private:
  // These really don't need to be exposed 
  void Render();
  void Update();
  void Input();
  
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

  // TODO: Remove later
  std::vector<Entity *> entities;

  glm::mat4 viewMatrix;
  ShaderProgram *program;
  Camera *camera;
  Transformation *t;

  Scene *scene;
};

#endif
