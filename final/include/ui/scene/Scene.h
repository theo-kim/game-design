#ifndef SCENE_DEF
#define SCENE_DEF

#include "standard.h"
#include "ui/entity/Entity.h"

// The base scene is an abstract class which just handles
// the logic behind rendering entities
class Scene {
public:
  Scene(SDL_Window* _window) : window(_window) {}
  virtual ~Scene() {}
  virtual void Render() = 0;
  virtual void Input(SDL_Event& e) = 0;
  virtual void Update(float delta) = 0;
protected:
  SDL_Window* window;
};

#endif