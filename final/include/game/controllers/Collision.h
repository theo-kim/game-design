#ifndef COLLISION_DEF
#define COLLISION_DEF

#include "Controller.h"
#include "game/playable/ControllableEntity.h"
#include "game/scenes/GameScene.h"

class ControllableEntity;
class GameScene;

class CollisionController : public Controller {
public:
  CollisionController(ControllableEntity *c, GameScene *scene);

  virtual void Update(float delta);
  virtual void Input(SDL_Event& e);
  virtual void ClearState();
private:
  ControllableEntity *object;
  std::vector<PhysicsEntity *> checked;
  GameScene *scene;
};

#endif