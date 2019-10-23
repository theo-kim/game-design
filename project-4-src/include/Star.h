#ifndef STAR_DEF
#define STAR_DEF

#include "standard.h"
#include "Entity.h"

class Star : public Entity {
 public:
  Star();
  Star(ShaderProgram *program, unsigned int seed, float l, float r, float t, float b);
  void Render();
  void Update(float delta);
 private:
  bool twinkleState;
};

#endif
