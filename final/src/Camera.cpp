#include "ui/camera/Camera.h"

Camera::Camera(Transformation *t, glm::mat4 p) 
  : transformation(t),
    perspective(p)
{}

glm::mat4 Camera::GetPerspectiveMatrix() const {
  return perspective;
}

glm::mat4 Camera::GetViewMatrix() const {
  return transformation->GetGlobalModelMatrix();
}

void Camera::Translate(Transformation::Translation translation) {
  transformation->LocalInverseTransform(-translation);
}

void Camera::Rotate(Transformation::Rotation rotation) {
  transformation->Transform(-rotation);
}

void Camera::BindToEntity(const Entity& e) {
  transformation = e.transformation;
}