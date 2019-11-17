#include "../include/Platform.h"

Platform::Platform(UntexturedShader *shader, CollisionEngine *e, glm::vec3 c) 
    : UntexturedEntity(shader, c),
      Collidable(e) {}

Platform::Platform(UntexturedShader *shader, CollisionEngine *e, glm::vec3 c, glm::vec3 pos, float width) 
    : UntexturedEntity(shader, c),
      Collidable(e),
      Entity(shader, pos, glm::vec3(width, 1.0, 1.0), 0.0f) { }

// Entity Implementation
void Platform::Render() {
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, GetPos());
  modelMatrix = glm::rotate(modelMatrix, GetRot(), glm::vec3(0.0f, 0.0f, 1.0f));
  modelMatrix = glm::scale(modelMatrix, GetSize()); 
  
  float map[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5,
          -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1 };

  UntexturedEntity::Render(modelMatrix, map, 12);
}
void Platform::Update(float delta) {}

// Collidable Implementation
void Platform::DidCollide(Collidable *with) {}
int Platform::CheckCollision(Collidable *with) {
  return QUADTREE_IGNORED_COLLISION;
}
bool Platform::DidUpdate() {
  return true;
}
Collidable::ColliderType Platform::GetColliderType() {
  return Collidable::SURFACE;
}