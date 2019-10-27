#include "include/Gun.h"

Gun::Gun() {}

Gun::Gun(ShaderProgram *program, Ship *_owner, Bullet temp, TextureSheet *texture)
  : Entity(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.6f, 0.6f, 1.0f), 0.0f, texture, program),
    owner(_owner)
{
  bulletTemplate = temp;
}

void Gun::Update(float delta) {
  for (int i = 0; i < activeBullets.size(); ++i) {
    if (activeBullets[i].IsAlive()) {
      activeBullets[i].Update(delta);
    }
    else {
      activeBullets.erase(activeBullets.begin() + i);
    }
  }
}

void Gun::Render() {
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  owner->TransformLocalCoord(modelMatrix);
  modelMatrix = glm::scale(modelMatrix, size);
  modelMatrix = glm::translate(modelMatrix, pos);
  modelMatrix = glm::rotate(modelMatrix, rot, glm::vec3(0.0f, 0.0f, 1.0f)); 
  
  program->SetColor(0.5f, 0.5f, 0.5f, 1.0f);

  float map[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5,
		  -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1 };

  texture->GetSprite(map + 12, 0, 0);
  
  Entity::Render(modelMatrix, texture->GetId(), map, 12);

  for (int i = 0; i < activeBullets.size(); ++i) {
    activeBullets[i].Render();
  }
}

void Gun::Rotate(float _rot) {
  rot = _rot;
}

void Gun::Fire() {
  for (int i = 0; i < firePoints.size(); ++i) {
    glm::vec3 origin = firePoints[i].GetLoc();
    glm::vec4 torigin = glm::vec4(origin[0], origin[1], origin[2], 1.0f);
    Bullet b = bulletTemplate;
    glm::mat4 transformation(1.0f);
    
    owner->TransformLocalCoord(transformation);
    TransformLocalCoord(transformation);

    torigin = torigin * transformation;
    origin = glm::vec3(torigin[0], torigin[1], torigin[2]);
    origin = origin + owner->GetPos();
    b.SetOrigin(origin, rot);
    activeBullets.push_back(b);
  }
}

DoubleGun::DoubleGun() {}

DoubleGun::DoubleGun(ShaderProgram **program, Ship *_owner, TextureSheet *texture)
  : Gun(program[0], _owner, Bullet(program[1], glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 10.0f, 10.0f, 10), texture)
{
  firePoints.push_back(VertexSensor(glm::vec3(0.3f, -0.09f, 1.0f), this));
  firePoints.push_back(VertexSensor(glm::vec3(0.3f, 0.09f, 1.0f), this));
}