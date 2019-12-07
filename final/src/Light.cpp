#include "ui/light/Light.h"

Light::Light(Transformation *position, float _power, glm::vec3 _color) 
  : transformation(position),
    power(_power),
    color(_color)
{}

float Light::GetPower() const {
  return power;
}

glm::vec3& Light::GetPosition() {
  pos = glm::vec3(transformation->GetModelMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  return pos;
}

glm::vec3& Light::GetColor() {
  return color;
}