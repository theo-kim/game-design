#ifndef LIGHT_DEF
#define LIGHT_DEF

#include "ui/transformation/Transformation.h"

class Light {
public:
  Light(Transformation *position, float power, glm::vec3 color);
  float GetPower() const;
  glm::vec3& GetPosition();
  glm::vec3& GetColor();
private:
  Transformation *transformation;
  float power;
  glm::vec3 color;
  glm::vec3 pos;
};

#endif