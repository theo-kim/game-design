#include "../include/Character.h"

Character::Character(ShaderProgram *_r, TextureSheet *_t, PhysicsEngine *_p, CollisionEngine *_c, float _sp, int _mH, float _m)
    : Entity(_r, _t),
      Collidable(_c),
      Physical(_p, _m),
      speed(_sp),
      maxHealth(_mH),
      currentHealth(_mH)
{}

void Character::Render() {
    if (currentHealth <= 0) return;
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, pos);
    modelMatrix = glm::rotate(modelMatrix, rot, glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, size); 
    
    float map[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5,
            -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1 };

    texture->GetSprite(map + 12, 0, 0);

    program->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    Entity::Render(modelMatrix, texture->GetId(), map, 12);
}

void Character::Update(float delta) {}

void Character::DidCollide(Collidable *with) {

}

int Character::CheckCollision(Collidable *with) {
    return QUADTREE_ILLEGAL_COLLISION;
}

bool Character::DidUpdate() {
    return false;
}

Collidable::ColliderType Character::GetColliderType() {
    return Collidable::CHARACTER;
}