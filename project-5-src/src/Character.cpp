#include "../include/Character.h"
#include "../include/Platform.h"

#define JUMP 600.0f

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
      ulting(false),
      p(glm::vec3(0.0f), 0, this)
{
    // glm::vec3 tl(0.0f);
    // glm::vec3 tr(0.0f);
    // glm::vec3 bl(0.0f);
    // glm::vec3 br(0.0f);

    glm::vec3 tl(-0.5f, 0.5f, 0.0f);
    glm::vec3 tr(0.5f, 0.5f, 0.0f);
    glm::vec3 bl(-0.5f, -0.5f, 0.0f);
    glm::vec3 br(0.5f, -0.5f, 0.0f);

    edges.push_back(EdgeSensor(tl, tr, this)); // Top - 0
    edges.push_back(EdgeSensor(tl, bl, this)); // Left - 1
    edges.push_back(EdgeSensor(tr, br, this)); // Right - 2

    edges.push_back(EdgeSensor(glm::vec3(-0.20f, -0.40f, 0.0f), glm::vec3(-0.20f, -0.52f, 0.0f), this)); // 3
    edges.push_back(EdgeSensor(glm::vec3(0.13f, -0.40f, 0.0f), glm::vec3(0.13f, -0.52f, 0.0f), this)); // 4

    // edges.push_back(EdgeSensor(glm::vec3(30.0f, -0.4f, 0.0f), glm::vec3(30.0f, -0.52f, 0.0f), this)); // 4

    scale = GetSize();
}

bool Character::GetGarbage() {
    return Entity::GetGarbage();
}

void Character::Walk(int doit) {
    walking = doit;
}
void Character::Jump(bool doit) {
    if (jumping == 0 && doit) {
        AddForce(glm::vec3(0.0f, 1.0f, 0.0f) * JUMP);
        jumping = 1;
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
    else if (walking != 0 && (animState == 0 || animState == 2) && accumulator > (1 - speed)) {
        animState = 1;
        accumulator -= (1 - speed);
    }
    else if (walking != 0 && animState == 1 && accumulator > (1 - speed)) {
        animState = 0;
        accumulator -= (1 - speed);
    }
    accumulator += delta;
}

void Character::DidCollide(Collidable *with) {
    if (with->GetColliderType() == Collidable::SURFACE) {
        Platform *p = dynamic_cast<Platform *>(with);
        if (edges[3].GetState() == 1 || edges[4].GetState() == 1) {
            if (p->edges[0].GetState() == 1) {
                AddForce(glm::vec3(0.0f, 1.0f, 0.0f) * GetPhysicsEngine()->GetGravity() * GetMass());
                if (mov[1] < 0) {
                    mov[1] = 0.0f;
                    jumping = 0;
                }
            }
        }
        else if (edges[1].GetState() == 1 && edges[2].GetState() == 1) {
            if (mov[1] > 0) {
                mov[1] = 0;
            }
        }
        else if (edges[1].GetState() == 1 || edges[2].GetState() == 1) {
            if (edges[2].GetState() == 1 && walking > 0) {
                Walk(0);
            }
            else if (edges[1].GetState() == 1 && walking < 0) {
                Walk(0);
            }
        }     
    }
    else if (with->GetColliderType() == Collidable::BALLISTIC) {
        currentHealth = 0;
    }
    else if (with->GetColliderType() == Collidable::CHARACTER) {
        if (edges[3].GetState() == 1 || edges[4].GetState() == 1) {}
        else if (edges[0].GetState() == 1 || edges[1].GetState() == 1 || edges[2].GetState() == 1) {
            currentHealth = 0;
        }
    }
}

int Character::CheckCollision(Collidable *with) {
    if (currentHealth <= 0) {
        SetGarbage();
        return QUADTREE_DEAD_ENTITY;
    }
    for (EdgeSensor &s : edges) {
        s.DecrementState();
    }
    if (with->GetColliderType() == Collidable::SURFACE) {
        return with->CheckCollision(this); // Have the platform check the collision
    }
    else if (with->GetColliderType() == Collidable::CHARACTER) {
        bool flag = false;
        Character *c = dynamic_cast<Character *>(with);
        if (c == NULL) return QUADTREE_ILLEGAL_COLLISION;
        for (int i = 0; i < edges.size(); ++i) {
            for (int j = 0; j < c->edges.size(); ++j) {
                if (edges[i].CheckCollision(&(c->edges[j]))) {
                    flag = true;
                }
            }
        }
        if (flag) return QUADTREE_YES_COLLISION;
        return QUADTREE_NO_COLLISION;
    }
    return QUADTREE_IGNORED_COLLISION;
}

bool Character::DidUpdate() {
    return true;
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