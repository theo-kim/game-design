#include "ui/entity/Entity.h"

Entity::Entity(Mesh *_mesh, Transformation *_transformation, ShaderProgram *_shader) 
  : mesh(_mesh),
    transformation(_transformation),
    shader(_shader)
{}

void Entity::Render() const {
  glm::mat4 modelMatrix = transformation->GetModelMatrix();
  shader->GenerateMVP(modelMatrix);
  mesh->Render(shader);
}

void Entity::Update(float delta) {

}

void Entity::Input() {
  
}

Transformation *Entity::CameraAttachPoint(Camera *camera) {
  return transformation;
}