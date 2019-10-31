#ifndef GAME_DEF
#define GAME_DEF

// Game Configuration Settings
#define GAME_NAME "Space War"
#define FIXED_TIMESTEP 0.01666f
#define NUM_STARS 300
#define NUM_ENEMIES 3

#include "standard.h"
#include "Text.h"
#include "Entity.h"
#include "Ship.h"
#include "Star.h"
#include "QuadTree.h"
#include "HeroShip.h"
#include "EvilShip.h"
#include "MiniMap.h"

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

  // Physics Methods

  // Physics Variables

  // Camera Methods
  void CameraPan(glm::vec3 pan);
  void CameraZoom(float zoomFactor);
  void CameraRotate(float rotationFactor);

  // Camera Variables
  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;
  glm::mat4 backgroundProjectionMatrix;
  glm::mat4 overlayProjectionMatrix;
  
  // Implementation Variables
  ShaderProgram rendererTextured; // foreground layer
  ShaderProgram rendererUntextured; // foreground layer
  ShaderProgram rendererUntexturedBackground; // background layer
  ShaderProgram rendererUntexturedOverlay; // background layer
  ShaderProgram rendererTexturedOverlay; // background layer
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

  float pTop;
  float pLeft;
  float pRight;
  float pBottom;

  // Collision tracking
  QuadTree collisionTree;
  
  // BEGIN GAME SPECIFIC SPACE
  // Methods
  void Spawn(int n, int difficulty);
  
  // States
  int fireState;
  
  // Entities
  std::vector<Entity *> foreground;
  std::vector<Entity *> background;
  std::vector<Entity *> overlay;

  HeroShip *ship;
  std::vector<EvilShip *> enemies;
  Star *stars[NUM_STARS];

  // Map
  //MiniMap map;
  
  // Text
  Text *endGameText;

  // Textures
  TextureSheet *heroTexture;
  TextureSheet *dualGunTexture;
  TextureSheet *enemyTexture;
  TextureSheet *singleGunTexture;

  // Game State
  int enemiesLeft;
};

#endif
