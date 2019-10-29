#include "include/Ship.h"

Ship::Ship() {}

Ship::Ship(ShaderProgram *_program, QuadTree *collisionEngine, TextureSheet *texture)
  : Collidable(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, texture, _program, collisionEngine),
    mov(glm::vec3(0.0f)),
    speed(1),
    health(10),
    prox(glm::vec3(0.0f), 1.0f, this),
    isMoving(0)
{}

Ship::Ship(ShaderProgram *_program, QuadTree *collisionEngine, glm::vec3 p, glm::vec3 s, TextureSheet *texture)
  : Collidable(p, s, 0.0f, texture, _program, collisionEngine),
    mov(glm::vec3(0.0f)),
    speed(1),
    health(10),
    prox(glm::vec3(0.0f), 1.0f, this),
    isMoving(0)
{}

Ship::~Ship() {}

void Ship::Update(float delta) {
  pos += mov * delta * speed;
  // Update in the collision tree
  Collidable::Update(delta);
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), mov * delta * speed);
  //prox.Update(transform);
  
  for (int i = 0; i < guns.size(); ++i) {
    guns[i].Update(delta);
  }
}

void Ship::Render() {
  if (health <= 0) return;
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, pos);
  modelMatrix = glm::scale(modelMatrix, size);
  
  float map[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5,
		  -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1 };

  // for (int i = 0; i < 4; ++i) {}
  
  texture->GetSprite(map + 12, 0, 0);

  program->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
  
  Entity::Render(modelMatrix, texture->GetId(), map, 12);

  for (int i = 0; i < guns.size(); ++i) {
    guns[i].Render();
  }
}

void Ship::Move(bool up, bool down, bool left, bool right) {
  isMoving = 0;
  isMoving = 0 | ((int)up << 3) | ((int)down << 2) | ((int)left << 1) | right;
  mov = glm::vec3((float)right - (float)left, (float)up - (float)down, 0.0f);
}

glm::vec3 Ship::GetMov() const {
  return mov * speed;
}

void Ship::Fire(int gunIndex) {
  guns[gunIndex].Fire();
}

bool Ship::DidUpdate() {
  return isMoving != 0;
}

void Ship::DidCollide(Collidable *with) {
  if (with->GetColliderType() == Collidable::BALLISTIC) {
    health = 0;
  }
}

int Ship::CheckCollision(Collidable *with) {
  if (health <= 0) {
    return QUADTREE_DEAD_ENTITY;
  }
  Collidable::ColliderType type = with->GetColliderType();
  if (type == Collidable::BALLISTIC) {
    Bullet * b = dynamic_cast<Bullet *>(with);
    if (b == NULL) return QUADTREE_ILLEGAL_COLLISION;
    // Now we have a bullet and this object
    if (b->shotBy->owner == this) {
      return QUADTREE_ILLEGAL_COLLISION;
    }
    // Step 1: Determine distance
    if (!prox.CheckCollision(b->GetProximitySensor())) {
      return QUADTREE_NO_COLLISION;
    }
    // Step 2: If near enough check actual collision

    return QUADTREE_YES_COLLISION;
  }
  return QUADTREE_ILLEGAL_COLLISION;
}

Collidable::ColliderType Ship::GetColliderType() {
  return Collidable::OBJECT;
}
