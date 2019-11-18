#include "../include/Platform.h"
#include "../include/Character.h"
#include "../include/CharacterTypes.h"

Platform::Platform(UntexturedShader *shader, CollisionEngine *e, glm::vec3 c) 
    : UntexturedEntity(shader, c),
      Collidable(e) {
        float w = GetSize()[0] / 2;
        float h = GetSize()[1] / 2;
        glm::vec3 tl(-0.5f, 0.5f, 0.0f);
        glm::vec3 tr(0.5f, 0.5f, 0.0f);
        glm::vec3 bl(-0.5f, -0.5f, 0.0f);
        glm::vec3 br(0.5f, -0.5f, 0.0f);

        edges.push_back(EdgeSensor(tl, tr, this));
        edges.push_back(EdgeSensor(bl, br, this));
        edges.push_back(EdgeSensor(tl, bl, this));
        edges.push_back(EdgeSensor(tr, br, this));
      }

Platform::Platform(UntexturedShader *shader, CollisionEngine *e, glm::vec3 c, glm::vec3 pos, float width) 
    : UntexturedEntity(shader, c),
      Collidable(e),
      Entity(shader, pos, glm::vec3(width, 0.2, 1.0), 0.0f) 
{
  float w = GetSize()[0] / 2;
  float h = GetSize()[1] / 2;
  glm::vec3 tl(-0.5f, 0.5f, 0.0f);
  glm::vec3 tr(0.5f, 0.5f, 0.0f);
  glm::vec3 bl(-0.5f, -0.5f, 0.0f);
  glm::vec3 br(0.5f, -0.5f, 0.0f);

  edges.push_back(EdgeSensor(tl, tr, this));
  edges.push_back(EdgeSensor(bl, br, this));
  edges.push_back(EdgeSensor(tl, bl, this));
  edges.push_back(EdgeSensor(tr, br, this));
}

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
void Platform::Update(float delta) {
  Collidable::Update(delta);
}

// Collidable Implementation
void Platform::DidCollide(Collidable *with) {
  return; // Platforms don't do anything...
}

int Platform::CheckCollision(Collidable *with) {
  if (with->GetColliderType() == Collidable::CHARACTER) {
    Character *c = dynamic_cast<Character *>(with);
    if (c == NULL) return QUADTREE_ILLEGAL_COLLISION;
    for (int i = 0; i < edges.size(); ++i) {
      for (int j = 0; j < c->edges.size(); ++j) {
        if (edges[i].CheckCollision(&(c->edges[j]))) {
          return QUADTREE_YES_COLLISION;
        }
      }
    }
    return QUADTREE_NO_COLLISION;
  }
  else if (with->GetColliderType() == Collidable::BALLISTIC) {
    Dwight::Beet *c = dynamic_cast<Dwight::Beet *>(with);
    if (c == NULL) return QUADTREE_ILLEGAL_COLLISION;
    for (int i = 0; i < edges.size(); ++i) {
      for (int j = 0; j < c->edges.size(); ++j) {
        if (edges[i].CheckCollision(&(c->edges[j]))) {
          return QUADTREE_YES_COLLISION;
        }
      }
    }
    return QUADTREE_NO_COLLISION;
  }
  return QUADTREE_ILLEGAL_COLLISION;
}
bool Platform::DidUpdate() {
  return false; // Doesn't move
}
Collidable::ColliderType Platform::GetColliderType() {
  return Collidable::SURFACE;
}
