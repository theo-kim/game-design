#ifndef GAMESCENE_DEF
#define GAMESCENE_DEF

#include "standard.h"
#include "ui/scene/Scene.h"
#include "game/environment/Planet.h"
#include "game/environment/Sun.h"
#include "game/playable/Ship.h"
#include "game/controllers/Controller.h"
#include "game/controllers/Human.h"
#include "ui/camera/Camera.h"
#include "ui/light/Light.h"

class GameScene : public Scene {
public:
  GameScene(float weight, float height);
  virtual ~GameScene();
  virtual void Render();
  virtual void Input();
  virtual void Update(float delta);
private:
  float height, width;
  Camera *camera;
  Planet *planet;
  Sun *sun;
  Human *player;
  std::vector<Ship *> ships;
  std::vector<Controller *> controllers;
};

#endif