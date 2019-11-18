#include "../include/CharacterTypes.h"

#define JIM_SPEED 0.75
#define STAN_SPEED 0.25
#define MIKE_SPEED 0.66
#define DWIGT_SPEED 0.66
#define JAN_SPEED 0.5

TextureSheet *Jim::Sprite = NULL;

Jim::Jim(TexturedShader *program, glm::vec3 pos)
    : Character(program, Jim::Sprite, NULL, NULL, 1, 1, 1),
      Entity(program, pos, glm::vec3(Jim::Sprite->GetSpriteSize() * 0.5f, 0.5, 1.0f), 0.0f)
{}

Jim::Jim(TexturedShader *program, glm::vec3 pos, float height)
    : Character(program, Jim::Sprite, NULL, NULL, 1, 1, 1),
      Entity(program, pos, glm::vec3(Jim::Sprite->GetSpriteSize() * height, height, 1.0f), 0.0f)
{}

Jim::Jim(TexturedShader *program, PhysicsEngine *phys, CollisionEngine *col, glm::vec3 pos, float height) 
    : Character(program, Sprite, phys, col, JIM_SPEED, 1, 1),
      Entity(program, pos, glm::vec3(Sprite->GetSpriteSize() * height, height, 1.0f), 0.0f)
{
  threshold = 0.0f;
}

void Jim::Ult(bool doit) {}

TextureSheet *Dwight::Sprite = NULL;

Dwight::Dwight(TexturedShader *program, glm::vec3 pos)
    : Character(program, Dwight::Sprite, NULL, NULL, 1, 1, 1),
      Entity(program, pos, glm::vec3(Dwight::Sprite->GetSpriteSize() * 0.5f, 0.5, 1.0f), 0.0f)
{}

Dwight::Dwight(TexturedShader *program, glm::vec3 pos, float height)
    : Character(program, Dwight::Sprite, NULL, NULL, 1, 1, 1),
      Entity(program, pos, glm::vec3(Dwight::Sprite->GetSpriteSize() * height, height, 1.0f), 0.0f)
{}

Dwight::Dwight(UntexturedShader *program, TexturedShader *_program, PhysicsEngine *phys, CollisionEngine *col, glm::vec3 pos, float height) 
    : Character(_program, Sprite, phys, col, DWIGT_SPEED, 1, 1),
      Entity(_program, pos, glm::vec3(Sprite->GetSpriteSize() * height, height, 1.0f), 0.0f),
      beetProgram(program)
{
  threshold = 6.0f;
}

void Dwight::Ult(bool doit) {
  ulting = ulting || doit;
}

void Dwight::Update(float delta) {
  if (ulting) {
    animState = 3;
    ultaccumulator += delta;
    if (ultaccumulator > 0.33f) {
      ultaccumulator = 0;
      ulting = false;
      glm::vec3 throwa(4.0f, 6.0f, 0.0f);
      if (scale[0] < 0) {
        throwa[0] *= -1;
      }
      // beets.empty();
      beets.push_back(new Dwight::Beet(this, throwa, GetPos()));
    }
  }
  Character::Update(delta);
  for (int i = 0; i < beets.size(); ++i) {
    Beet *b = beets[i];
    if (b->kill) {
      delete b;
      beets.clear();
    }
    else {
      b->Update(delta); 
    }
  };
}

void Dwight::Render() {
  Character::Render();
  for (int i = 0; i < beets.size(); ++i) {
    Beet *b = beets[i];
    b->Render();
  };
}

Dwight::Beet::Beet(Dwight *_owner, glm::vec3 vel, glm::vec3 origin)
  : Rect(_owner->beetProgram, origin, glm::vec3(0.05f, 0.05f, 1.0f), 0.0f, glm::vec3(0.8f, 0.1f, 0.4f)),
    Entity(_owner->beetProgram, origin, glm::vec3(0.05f, 0.05f, 1.0f), 0.0f),
    velocity(0.0f),
    owner(_owner),
    kill(false),
    readytodie(false),
    Collidable(_owner->GetCollisionEngine()),
    Physical(_owner->GetPhysicsEngine(), 0.1f) {
      AddForce(vel);
      glm::vec3 tl(-0.5f, 0.5f, 0.0f);
      glm::vec3 tr(0.5f, 0.5f, 0.0f);
      glm::vec3 bl(-0.5f, -0.5f, 0.0f);
      glm::vec3 br(0.5f, -0.5f, 0.0f);

      edges.push_back(EdgeSensor(tl, tr, this));
      edges.push_back(EdgeSensor(bl, br, this));
      edges.push_back(EdgeSensor(tl, bl, this));
      edges.push_back(EdgeSensor(tr, br, this));
    }

void Dwight::Beet::Update(float delta) {
  Collidable::Update(delta);
  Physical::Update(delta);

  velocity += GetForce() / GetMass() * delta;
  AddPos(velocity * delta);
  SetForce(glm::vec3(0.0f));
}

void Dwight::Beet::GravitateTo(glm::vec3 direction, float g) {
    AddForce(direction * g * GetMass());
}

glm::vec3 Dwight::Beet::Push() const { return GetForce(); };
void Dwight::Beet::PushBy(glm::vec3 force) {};

void Dwight::Beet::DidCollide(Collidable *with) {
  readytodie = true;
}
int Dwight::Beet::CheckCollision(Collidable *with) {
  if (readytodie) {
    kill = true;
    return QUADTREE_DEAD_ENTITY;
  }
  if (with->GetColliderType() == SURFACE) {
    return with->CheckCollision(this);
  }
  else if (with->GetColliderType() == CHARACTER) {
    if (with == owner) return QUADTREE_IGNORED_COLLISION;
    Jan *c = dynamic_cast<Jan *>(with);
    if (c == NULL) {
      return QUADTREE_ILLEGAL_COLLISION;
    }
    for (int i = 0; i < edges.size(); ++i) {
      for (int j = 0; j < c->edges.size(); ++j) {
        if (edges[i].CheckCollision(&(c->edges[j]))) {
          return QUADTREE_YES_COLLISION;
        }
      }
    }
  }
}
bool Dwight::Beet::DidUpdate() {
  return true;
}
Collidable::ColliderType Dwight::Beet::GetColliderType() {
  return BALLISTIC;
}

TextureSheet *Michael::Sprite = NULL;

Michael::Michael(TexturedShader *program, glm::vec3 pos)
    : Character(program, Michael::Sprite, NULL, NULL, 1, 1, 1),
      Entity(program, pos, glm::vec3(Michael::Sprite->GetSpriteSize() * 0.5f, 0.5, 1.0f), 0.0f)
{}

Michael::Michael(TexturedShader *program, glm::vec3 pos, float height)
    : Character(program, Michael::Sprite, NULL, NULL, 1, 1, 1),
      Entity(program, pos, glm::vec3(Michael::Sprite->GetSpriteSize() * height, height, 1.0f), 0.0f)
{}

Michael::Michael(TexturedShader *program, PhysicsEngine *phys, CollisionEngine *col, glm::vec3 pos, float height) 
    : Character(program, Sprite, phys, col, MIKE_SPEED, 1, 1),
      Entity(program, pos, glm::vec3(Sprite->GetSpriteSize() * height, height, 1.0f), 0.0f)
{
  threshold = 30.0f;
}

void Michael::Ult(bool doit) {
  ulting = ulting || doit;
}

void Michael::Update(float delta) {
  if (ulting) {
    if (walking == 0 && jumping == 0) {
        animState = 3;
        accumulator = 1 - GetSpeed();
    }
    else if (jumping > 0) {
        animState = 5;
        accumulator = 0;
    }
    else if (walking == 1 && animState == 0 && accumulator > (1 - GetSpeed())) {
        animState = 4;
        accumulator = 0;
    }
    else if (walking == 1 && animState == 1 && accumulator > (1 - GetSpeed())) {
        animState = 3;
        accumulator = 0;
    }
    accumulator += delta;
    ultaccumulator += delta;
    if (ultaccumulator > 5.0f) {
      ultaccumulator = 0.0f;
      ulting = false;
    }
  }
  Character::Update(delta);
}

TextureSheet *Stanley::Sprite = NULL;

Stanley::Stanley(TexturedShader *program, glm::vec3 pos)
    : Character(program, Stanley::Sprite, NULL, NULL, 1, 1, 1),
      Entity(program, pos, glm::vec3(Stanley::Sprite->GetSpriteSize() * 0.5f, 0.5, 1.0f), 0.0f)
{}

Stanley::Stanley(TexturedShader *program, glm::vec3 pos, float height)
    : Character(program, Stanley::Sprite, NULL, NULL, 1, 1, 1),
      Entity(program, pos, glm::vec3(Stanley::Sprite->GetSpriteSize() * height, height, 1.0f), 0.0f)
{}

Stanley::Stanley(TexturedShader *program, PhysicsEngine *phys, CollisionEngine *col, glm::vec3 pos, float height) 
    : Character(program, Stanley::Sprite, phys, col, STAN_SPEED, 1, 1),
      Entity(program, pos, glm::vec3(Stanley::Sprite->GetSpriteSize() * height, height, 1.0f), 0.0f)
{
  threshold = 45.0f;
  effect = Mix_LoadWAV( "textures/didistutter.wav" );
}

void Stanley::Ult(bool doit) {
  if (!ulting && doit) {
    Mix_PlayChannel( -1, effect, 0 );
  }
  ulting = ulting || doit;
  if (ulting) {
    walking = 0;
    jumping = 0;
    animState = 3;
  }
}

void Stanley::Update(float delta) {
  if (ulting) {
    animState = 3;
    ultaccumulator += delta;
    if (ultaccumulator > 1.0f) {
      ultaccumulator = 0;
      ulting = false;
    }
  }
  else {
    Character::Update(delta);
  }
}

TextureSheet *Jan::Sprite = NULL;

Jan::Jan(TexturedShader *program, PhysicsEngine *phys, CollisionEngine *col, glm::vec3 pos, float height) 
  : Character(program, Sprite, phys, col, JAN_SPEED, 1, 1),
      Entity(program, pos, glm::vec3(Stanley::Sprite->GetSpriteSize() * height, height, 1.0f), 0.0f) {}

void Jan::Update(float delta) {
  if (acc > 1.0f && acc <= 2.0f) Walk(-1.0f);
  else if (acc <= 1.0f) Walk(1.0f);
  else if (acc <= 3.0f) Walk(1.0f);
  else {
    acc = 1.0f;
  }
  acc += delta;
  Character::Update(delta);
}

int Jan::CheckCollision(Collidable *with) {
  if (currentHealth <= 0) return QUADTREE_DEAD_ENTITY;
  return with->CheckCollision(this);
}

void Jan::DidCollide(Collidable *with) {
  if (with->GetColliderType() == Collidable::SURFACE) {
    AddForce(glm::vec3(0.0f, 1.0f, 0.0f) * GetPhysicsEngine()->GetGravity() * GetMass());
    
    jumping -= 1;
    if (jumping < 0) jumping = 0;
    if (jumping == 0) mov[1] = 0.0f;
  }
  else if (with->GetColliderType() == BALLISTIC) {
    currentHealth = 0;
  }
  else if (with->GetColliderType() == CHARACTER) {
    Character *c = dynamic_cast<Character *>(with);
    if (c == NULL) return;
    if (GetPos()[1] < c->GetPos()[1]) {
      currentHealth = 0;
    }
  }
}