#include "include/Gun.h"

Gun::Gun() {}

Gun::Gun(ShaderProgram *program, Ship *_owner, Bullet temp, TextureSheet *texture)
  : Entity(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.6f, 0.6f, 1.0f), 0.0f, texture, program),
    owner(_owner)
{
  bulletTemplate.push_back(Bullet(temp));
}

Gun::Gun(ShaderProgram *program, Ship *_owner, Bullet temp, TextureSheet *texture, glm::vec3 pos, glm::vec3 size)
  : Entity(pos, size, 0.0f, texture, program),
    owner(_owner)
{
  bulletTemplate.push_back(Bullet(temp));
}

Gun::~Gun() {
  for (int i = 0; i < activeBullets.size(); ++i) {
    delete activeBullets[i];
  }
}

void Gun::Update(float delta) {
  for (int i = 0; i < activeBullets.size(); ++i) {
    if (activeBullets[i]->IsAlive()) {
      activeBullets[i]->Update(delta);
    }
    else if (activeBullets[i]->ReadyToDie()) {
      delete activeBullets[i];
      activeBullets.erase(activeBullets.begin() + i);
    }
  }
}

void Gun::Render() {  
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  owner->TransformLocalCoord(modelMatrix);;

  modelMatrix = glm::translate(modelMatrix, pos);
  modelMatrix = glm::rotate(modelMatrix, rot, glm::vec3(0.0f, 0.0f, 1.0f)); 
  modelMatrix = glm::scale(modelMatrix, size);
  
  float map[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5,
		  -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1 };

  texture->GetSprite(map + 12, 0, 0);
  Entity::Render(modelMatrix, texture->GetId(), map, 12);

  for (int i = 0; i < activeBullets.size(); ++i) {
    if (activeBullets[i]->IsAlive()) {
      activeBullets[i]->Render();
    }
  }
}

void Gun::Rotate(float _rot) {
  rot = _rot;
}

void Gun::Fire() {
  for (int i = 0; i < firePoints.size(); ++i) {
    glm::vec3 origin = firePoints[i].GetLoc();
    glm::vec4 torigin(origin, 1.0f);
    glm::mat4 trans(1.0f);
    
    owner->TransformLocalCoord(trans);
    TransformLocalCoord(trans);

    origin = glm::vec3(trans * torigin);

    Bullet *b = new Bullet(bulletTemplate[0]);
    b->SetOrigin(origin, owner->TransformLocalRot(rot));
    b->shotBy = this;
    activeBullets.push_back(b);
  }
}

TextureSheet *DoubleGun::doubleGunTexture = new TextureSheet("textures/dual-gun.png", 1, 1);

DoubleGun::DoubleGun() {}

DoubleGun::DoubleGun(ShaderProgram **program, Ship *_owner, QuadTree *engine)
  : Gun(program[0], _owner, Bullet(program[1], engine, glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 10.0f, 10.0f, 10), doubleGunTexture)
{
  firePoints.push_back(VertexSensor(glm::vec3(0.25f, -0.15f, 1.0f), this));
  firePoints.push_back(VertexSensor(glm::vec3(0.25f, 0.15f, 1.0f), this));
}

TextureSheet *SingleGun::singleGunTexture = new TextureSheet("textures/single-gun.png", 1, 1);

SingleGun::SingleGun() {}

SingleGun::SingleGun(ShaderProgram **program, Ship *_owner, QuadTree *engine)
  : Gun(program[0], _owner, Bullet(program[1], engine, glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 5.0f, 20.0f, 10), singleGunTexture)
{
  firePoints.push_back(VertexSensor(glm::vec3(0.25f, 0.0f, 1.0f), this));
}

SingleGun::SingleGun(ShaderProgram **program, Ship *_owner, QuadTree *engine, glm::vec3 _pos, glm::vec3 _size, float rot)
  : Gun(program[0], _owner, Bullet(program[1], engine, glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 5.0f, 20.0f, 10), singleGunTexture, _pos, _size)
{
  firePoints.push_back(VertexSensor(glm::vec3(0.25f, 0.0f, 1.0f), this));
  Rotate(rot);
}
