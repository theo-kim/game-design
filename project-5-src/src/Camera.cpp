#include "../include/Camera.h"

Camera::Camera(glm::vec3 center, float _height, float _width, float initialZoom)
    : focus(center),
      zoom(initialZoom),
      width(_width),
      height(_height),
      following(false),
      target(NULL) {}

Camera::CameraState Camera::Update() {
    if (following) {
        PanTo(target->GetPos());
    }
}

void Camera::PanBy(glm::vec3 by) {
    projection = glm::translate(projection, -1.0f * by);
}

void Camera::PanTo(glm::vec3 to) {
    projection = glm::ortho(to[0] - (width / 2), to[0] + (width / 2), 
                            to[1] - (height / 2), to[1] + (height / 2), 
                            -1.0f, 1.0f);
    focus -= to;
}

void Camera::Zoom(glm::vec3 by) {
    projection = glm::scale(projection, glm::vec3(by, by, 1.0f));
}

void Camera::Shake() {

}

void Camera::Follow(Entity *e) {
    following = true;
    target = e;
}

bool Camera::IsVisible(Entity *e) {
    float left = focus[0] - (width / 2);
    float right = focus[0] + (width / 2);
    float top = focus[1] + (height / 2);
    float bottom = focus[1] - (height / 2);

    return e->CheckRenderBounds(left, right, top, bottom);
}

// Getters
glm::mat4 Camera::GetProjection() {
    return projection;
}