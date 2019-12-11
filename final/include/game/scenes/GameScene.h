#ifndef GAMESCENE_DEF
#define GAMESCENE_DEF

#include "standard.h"
#include "ui/scene/Scene.h"
#include "game/environment/Planet.h"
#include "game/environment/Sun.h"
#include "game/environment/Asteroid.h"
#include "game/playable/Ship.h"
#include "game/controllers/Controller.h"
#include "game/controllers/Human.h"
#include "ui/camera/Camera.h"
#include "ui/light/Light.h"
#include "ui/entity/Text.h"
#include "game/environment/Station.h"

class Ship;
class Asteroid;

class GameScene : public Scene {
public:
  GameScene(SDL_Window* window, float weight, float height);
  virtual ~GameScene();
  virtual void Render();
  virtual void Input(SDL_Event& );
  virtual void Update(float delta);

  std::vector<Ship *> ships;
  std::vector<Asteroid *> asteroids;
  Station *station;
private:
  std::vector<Entity *> overlay;
  float height, width;
  Camera *camera;
  Camera *cameraOverlay;
  Planet *planet;
  Sun *sun;
  std::vector<Controller *> controllers;

  Text *test;
  Text *welcome;

  bool ready;

  // Music
  Mix_Music *music;
};

#endif