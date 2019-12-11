#include "ui/camera/Camera.h"
#include <iomanip> 

Camera::Camera(Transformation *t, glm::mat4 p) 
  : transformation(t),
    perspective(p),
    following(false),
    careful(false)
{}

Camera::~Camera() {
  if (!following && !careful) {
    delete transformation;
  }
  else if (!careful) {
    delete offset;
  }
}
glm::mat4 Camera::GetPerspectiveMatrix() const {
  return perspective;
}

glm::mat4 Camera::GetViewMatrix() const {
  // std::cout << transformation->translation.vector[2] << std::endl;
  glm::mat4 rotations = glm::rotate(glm::mat4(1.0f), transformation->rotation.x * -1, glm::vec3(1, 0, 0)) * 
                        glm::rotate(glm::mat4(1.0f), transformation->rotation.y * -1, glm::vec3(0, 1, 0)) * 
                        glm::rotate(glm::mat4(1.0f), transformation->rotation.z * -1, glm::vec3(0, 0, 1));

  if (following) {
    glm::quat offsetQuat = glm::quat(glm::vec3(-transformation->rotation.x, -transformation->rotation.y, -transformation->rotation.z));
    glm::quat offsetLookQuat = glm::quat(glm::vec3(-transformation->rotation.x + -offset->rotation.x, -transformation->rotation.y + -offset->rotation.y, -transformation->rotation.z + -offset->rotation.z));
    
    glm::vec3 offsetLocalPosition = glm::vec3(glm::vec4(offset->translation.vector, 0.0f) * (offsetQuat));
    glm::vec3 cameraPosition = transformation->translation.vector + offsetLocalPosition;
    glm::vec3 lookingAt = cameraPosition + glm::vec3(glm::vec4(0, 0, -1.0f, 0.0f) * offsetLookQuat);
    return glm::lookAt(cameraPosition,
                      lookingAt,
                      glm::vec3(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f) * offsetQuat)
                      );
  }

  // std::cout << "s" << std::endl;
  return glm::mat4(1.0f);
  // return  transformation->scale.GetMatrix() * 
  //         rotations *
  //         glm::translate(glm::mat4(1.0f), 
  //           (transformation->translation.vector) * -1.0f);
}

void Camera::Translate(Transformation::Translation translation) {
  if (!following) {
    transformation->LocalInverseTransform(translation);
  }
  else {
    offset->LocalInverseTransform(translation);
  }
}

void Camera::Rotate(Transformation::Rotation rotation) {
  if (!following) {
    transformation->Transform(rotation);
  }
  else {
    offset->Transform(rotation);
    if (constraints[X][2] == 5.0f) {
      if (offset->rotation.x < constraints[X][0]) {
        offset->rotation.x = constraints[X][0];
      }
      else if (offset->rotation.x > constraints[X][1]) {
        offset->rotation.x = constraints[X][1];
      }
    }
    if (constraints[Y][2] == 5.0f) {
      if (offset->rotation.y < constraints[Y][0]) {
        offset->rotation.y = constraints[Y][0];
      }
      else if (offset->rotation.y > constraints[Y][1]) {
        offset->rotation.y = constraints[Y][1];
      }
    }
    if (constraints[Z][2] == 5.0f) {
      if (offset->rotation.z < constraints[Z][0]) {
        offset->rotation.z = constraints[Z][0];
      }
      else if (offset->rotation.z > constraints[Z][1]) {
        offset->rotation.z = constraints[Z][1];
      }
    }
  } 
}

void Camera::ConstrainRotation(Axis a, float min, float max) {
  constraints[a][0] = min;
  constraints[a][1] = max;
  constraints[a][2] = 5.0f;
}

void Camera::BindToEntity(Entity& e, Transformation _offset) {
  Transformation *t = e.CameraAttachPoint(this);
  if (t != nullptr) {
    if (!following) {
      offset = new Transformation(_offset);
      if (!careful) delete transformation;
    }
    else {
      if (!careful) delete offset;
      offset = new Transformation(_offset);
    }
    transformation = t;
    following = true;
    careful = false;
  }
}

void Camera::BindToTransformation(Transformation *t, Transformation *o) {
  if (!following) {
    offset = o;  
    if (!careful) delete transformation;
    transformation = t;
    following = true;
    careful = true;
  }
  else {
    if (!careful) delete offset;
    offset = o;
    transformation = t;
    careful = true;
  }
}

void Camera::Dettach() {
  following = false;
  transformation = offset;
}