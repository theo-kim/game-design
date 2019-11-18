#include "../include/Character.h"
#include "../include/Platform.h"

Character::Character(TexturedShader *_r, TextureSheet *_t, PhysicsEngine *_p, CollisionEngine *_c, float _sp, int _mH, float _m)
    : TexturedEntity(_r, _t),
      Entity(_r),
      Collidable(_c),
      Physical(_p, _m),
      speed(_sp),
      maxHealth(_mH),
      currentHealth(_mH),
      mov(0.0f),
      walking(0),
      animState(0),
      jumping(0),
      ulting(false)
{
    glm::vec3 tl(-0.5f, 0.5f, 0.0f);
    glm::vec3 tr(0.5f, 0.5f, 0.0f);
    glm::vec3 bl(-0.5f, -0.5f, 0.0f);
    glm::vec3 br(0.5f, -0.5f, 0.0f);

    edges.push_back(EdgeSensor(tl, tr, this));
    edges.push_back(EdgeSensor(bl, br, this));
    edges.push_back(EdgeSensor(tl, bl, this));
    edges.push_back(EdgeSensor(tr, br, this));

    scale = GetSize();
}

void Character::Walk(int doit) {
    walking = doit;
}
void Character::Jump(bool doit) {
    if (jumping == 0 && doit) {
        AddForce(glm::vec3(0.0f, 1.0f, 0.0f) * 50.0f);
        jumping = 2;
    }
}
void Character::Stop() {
    walking = 0;
}

void Character::Ult(bool doit) {}

void Character::Render() {
    if (currentHealth <= 0) return;
    
    if (walking < 0) {
        scale[0] = GetSize()[0] * -1;
    }
    else if (walking > 0) {
        scale[0] = GetSize()[0];
    }

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, GetPos());
    modelMatrix = glm::rotate(modelMatrix, GetRot(), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale); 
    
    float map[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    GetTexture()->GetSprite(map + 12, animState, 0);

    // program->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    TexturedEntity::Render(modelMatrix, map, 12);
}

void Character::Update(float delta) {
    if (GetCollisionEngine() == NULL) return;
    if (GetPhysicsEngine() == NULL) return;

    Collidable::Update(delta);
    Physical::Update(delta);

    mov += GetForce() / GetMass() * delta;
    mov[0] = walking * speed;
    AddPos(mov * delta);
    SetForce(glm::vec3(0.0f));

    if (ulting) {
        accumulator = 0;
    }
    else if (walking == 0 && jumping == 0) {
        animState = 0;
        accumulator = 1 - speed;
    }
    else if (jumping > 0) {
        animState = 2;
        accumulator = 0;
    }
    else if (walking == 1 && animState == 0 && accumulator > (1 - speed)) {
        animState = 1;
        accumulator = 0;
    }
    else if (walking == 1 && animState == 1 && accumulator > (1 - speed)) {
        animState = 0;
        accumulator = 0;
    }
    accumulator += delta;
}

void Character::DidCollide(Collidable *with) {
    if (with->GetColliderType() == Collidable::SURFACE) {
        AddForce(glm::vec3(0.0f, 1.0f, 0.0f) * GetPhysicsEngine()->GetGravity() * GetMass());
        
        jumping -= 1;
        if (jumping < 0) jumping = 0;
        if (jumping == 0) mov[1] = 0.0f;
    }
}

int Character::CheckCollision(Collidable *with) {
    if (with->GetColliderType() == Collidable::SURFACE) {
        return with->CheckCollision(this); // Have the platform check the collision
    }
    return QUADTREE_IGNORED_COLLISION;
}

bool Character::DidUpdate() {
    return false;
}

Collidable::ColliderType Character::GetColliderType() {
    return Collidable::CHARACTER;
}

glm::vec3 Character::Push() const {
    return glm::vec3(0.0f);
}
void Character::PushBy(glm::vec3 force) {
    AddForce(force);
}

void Character::GravitateTo(glm::vec3 direction, float g) {
    AddForce(direction * g * GetMass());
}

float Character::GetSpeed() const {
    return speed;
}