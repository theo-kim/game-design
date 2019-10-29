#include "include/Bullet.h"

Bullet::Bullet() {}

Bullet::Bullet(ShaderProgram *program, QuadTree *collisionEngine, glm::vec3 origin, glm::vec3 c, float s, float p, int pow)
  : Collidable(origin, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, program, collisionEngine),
    power(pow),
    speed(s),
    penetration(p),
    mov(glm::vec3(1.0f, 0.0f, 0.0f)),
    r(c[0]),
    g(c[1]),
    b(c[2]),
    alive(true),
    killme(false),
    prox(origin, 0.1, this),
    shotBy(NULL)
{}

Bullet::Bullet(const Bullet &copied)
  : Collidable(copied),
    power(copied.power),
    speed(copied.speed),
    penetration(copied.penetration),
    mov(glm::vec3(0.0f)),
    r(copied.r),
    g(copied.g),
    b(copied.b),
    alive(true),
    killme(false),
    prox(copied.pos, 0.1, this),
    shotBy(copied.shotBy)
{}

Bullet::~Bullet() {}

void Bullet::Update(float delta) {
  pos += mov * speed * delta;
  penetration -= 0.5;
  if (penetration < 0) {
    alive = false;
  }
  Collidable::Update(delta);
  glm::mat4 transform = glm::mat4(1.0f);
  prox.Update(transform);
}

void Bullet::Render() {
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, pos);
  modelMatrix = glm::rotate(modelMatrix, rot, glm::vec3(0.0f, 0.0f, 1.0f));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.05f, 1.0f));

  program->SetColor(r, g, b, 1.0f);

  float map[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5,
                  -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1 };
  
  Entity::Render(modelMatrix, (GLuint)NULL, map, 12);
}

void Bullet::SetOrigin(glm::vec3 _pos, float _rot) {
  mov = glm::vec3(glm::cos(_rot), glm::sin(_rot), 0.0f);
  rot = _rot;
  pos = _pos;
}

bool Bullet::IsAlive() {
  return alive;
}

bool Bullet::ReadyToDie() {
  return killme;
}

bool Bullet::DidUpdate() {
  return alive;
}

void Bullet::DidCollide(Collidable *with) {
  alive = false; // kill the bullet when it collides
}

int Bullet::CheckCollision(Collidable *with) {
  if (!alive) {
    killme = true;
    return QUADTREE_DEAD_ENTITY;
  }
  Collidable::ColliderType type = with->GetColliderType();
  if (type == Collidable::BALLISTIC) {
    return QUADTREE_ILLEGAL_COLLISION;
  }
  return with->CheckCollision((Bullet *)this); // Bullets do not handle their own collisions;
}

Collidable::ColliderType Bullet::GetColliderType() {
  return Collidable::BALLISTIC;
}

ProximitySensor *Bullet::GetProximitySensor() {
  return &prox;
}
