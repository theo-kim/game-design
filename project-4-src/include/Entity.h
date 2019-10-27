#ifndef ENTITY_DEF
#define ENTITY_DEF

#include "standard.h"

class Entity {
 public:
  Entity();
  Entity(glm::vec3 _pos, glm::vec3 _size, float _rot, ShaderProgram* _program);
  Entity(glm::vec3 _pos, glm::vec3 _size, float _rot, TextureSheet *texture, ShaderProgram* _program);
  virtual void Render(glm::mat4 modelMatrix, GLuint texture, float* map, int points);
  virtual void Update(float delta) = 0;
  bool CheckBounds(float left, float right, float top, float bottom);
  void SetRenderFlag(bool val);
  glm::vec3 GetPos () const;
  glm::mat4 GetCorners () const;

  // Utility functions to transform member bodies / points
  void TransformLocalCoord (glm::vec3 &point) const;
  void TransformLocalCoord (glm::mat4 &body) const;
protected:
  glm::vec3 pos;
  glm::vec3 size;
  float rot;
  ShaderProgram* program;
  bool doRender;
  TextureSheet *texture;
};

#endif
