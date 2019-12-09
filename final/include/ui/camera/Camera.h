#ifndef CAMERA_DEF
#define CAMERA_DEF

#include "standard.h"
#include "ui/transformation/Transformation.h"
#include "ui/entity/Entity.h"

class Camera {
public:
  Camera(Transformation *t, glm::mat4 perspective);
  glm::mat4 GetViewMatrix() const;
  glm::mat4 GetPerspectiveMatrix() const;
  
  // High level movements
  void LookAt(glm::vec3 target);
  void MoveTo(glm::vec3 position);
  
  // Low level movements
  void Rotate(Transformation::Rotation rotation);
  void Translate(Transformation::Translation translation);

  void BindToEntity(const Entity& e);
private:
  Transformation *transformation;
  glm::mat4 perspective;
};

#endif