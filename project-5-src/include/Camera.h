#ifndef CAMERA_DEF
#define CAMERA_DEF

#include "standard.h"
#include "Entity.h"

class Camera {
public:
    enum CameraState { UNCHANGED, CHANGED };

    Camera(glm::vec3 center, float height, float width, float initialZoom);

    CameraState Update();
    void PanBy(glm::vec3 by);
    void PanTo(glm::vec3 to);
    void Zoom(float by);
    void Shake();
    void Follow(Entity *e);

    bool IsVisible(Entity *e);

    // Getters
    glm::mat4 GetProjection();
private:
    glm::mat4 projection;
    glm::vec3 focus;
    float height, width;
    float zoom;

    bool following;
    Entity *target;
};

#endif