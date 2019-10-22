#ifndef ENTITY_DEF
#define ENTITY_DEF

#include "standard.h"

class Entity {
 public:
  Entity();
  Entity(glm::vec3 _pos, glm::vec3 _size, float _rot, ShaderProgram* _program);
  virtual void Render(glm::mat4 modelMatrix, GLuint texture, float* map, int points);
  virtual void Update() = 0;
 protected:
  glm::vec3 pos;
  glm::vec3 size;
  float rot;
  ShaderProgram* program;
};

#endif
