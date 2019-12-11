#ifndef SCENE_DEF
#define SCENE_DEF

#include "standard.h"
#include "ui/entity/Entity.h"

// The base scene is an abstract class which just handles
// the logic behind rendering entities
class Scene {
public:
  Scene() {}
  virtual ~Scene() {}
  virtual void Render() = 0;
  virtual void Input() = 0;
  virtual void Update(float delta) = 0;
};

#endif