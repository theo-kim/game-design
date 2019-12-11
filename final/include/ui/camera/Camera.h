#ifndef CAMERA_DEF
#define CAMERA_DEF

#include "standard.h"
#include "ui/transformation/Transformation.h"
#include "ui/entity/Entity.h"

class Entity;

class Camera {
public:
  enum Axis { X, Y, Z };

  Camera(Transformation *t, glm::mat4 perspective);
  ~Camera();
  glm::mat4 GetViewMatrix() const;
  glm::mat4 GetPerspectiveMatrix() const;
  
  // High level movements
  void LookAt(glm::vec3 target);
  void MoveTo(glm::vec3 position);
  
  // Low level movements
  void Rotate(Transformation::Rotation rotation);
  void Translate(Transformation::Translation translation);

  void ConstrainRotation(Axis a, float min, float max);

  void BindToEntity(Entity& e, Transformation offset);
  void BindToTransformation(Transformation *t, Transformation *o);
  void Dettach();
private:
  Transformation *transformation;
  Transformation *offset;
  glm::mat4 perspective;
  bool following;
  bool careful;
  glm::mat3 constraints;
};

#endif