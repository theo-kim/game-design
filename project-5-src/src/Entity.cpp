#include "../include/framework/Entity.h"
#include <math.h>

//
// BEGIN ENTITY DECLARATION:
Entity::Entity() {};

Entity::Entity(ShaderProgram *_program, glm::vec3 _pos, glm::vec3 _size, float _rot)
  : pos(_pos), size(_size), rot(_rot), program(_program), doRender(true), killme(false)
{}

Entity::Entity(ShaderProgram *_program)
  : pos(0.0f), size(1.0f), rot(0.0f), program(_program), doRender(true), killme(false)
{}

Entity::~Entity() {}

void Entity::Render(glm::mat4 modelMatrix, GLuint texture, float* map, int points) const {
  if (!doRender) return;
  program->SetModelMatrix(modelMatrix);

  glBindTexture(GL_TEXTURE_2D, texture);
 
  glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, map);
  glEnableVertexAttribArray(program->positionAttribute);

  glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, map + points);
  glEnableVertexAttribArray(program->texCoordAttribute);

  glDrawArrays(GL_TRIANGLES, 0, points / 2);

  glDisableVertexAttribArray(program->positionAttribute);
  glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::SetRenderFlag(bool flag) {
  doRender = flag;
}

bool Entity::CheckRenderBounds(float left, float right, float top, float bottom) {
  return (pos[0] + size[0] / 2  > left
	  || pos[0] - size[0] / 2 < right)
			      && (pos[1] + size[1] / 2 > bottom
				  || pos[1] - size[1] / 2 < top);
}

bool Entity::CheckBounds(float left, float right, float top, float bottom) {
  return pos[0] - size[0] / 2  > left
    && pos[0] + size[0] / 2 < right
    && pos[1] - size[1] / 2 > bottom
    && pos[1] + size[1] / 2 < top;
}

//Getters:

glm::vec3 Entity::GetPos () const {
  return pos;
}

glm::mat4x3 Entity::GetCorners () const {
  glm::vec4 topLeft(-0.5f, 0.5f, 1.0f, 1.0f);
  glm::vec4 topRight(0.5f, 0.5f, 1.0f, 1.0f);
  glm::vec4 bottomLeft(-0.5f, -0.5f, 1.0f, 1.0f);
  glm::vec4 bottomRight(0.5f, -0.5f, 1.0f, 1.0f);

  glm::mat4 transformation(1.0f);
  TransformLocalCoord(transformation);

  return glm::mat4x3(glm::vec3(transformation * topLeft), 
                     glm::vec3(transformation * topRight), 
                     glm::vec3(transformation * bottomLeft), 
                     glm::vec3(transformation * bottomRight));
}

bool Entity::GetGarbage() const { return killme; }

glm::vec3 Entity::GetSize () const { return size; }

float Entity::GetRot () const { return rot; }

ShaderProgram* Entity::GetProgram() const { return program; }

// Setters:
void Entity::SetGarbage() { killme = true; }
void Entity::AddPos(glm::vec3 _addition) { pos += _addition; }
void Entity::SetPos(glm::vec3 _pos) { pos = _pos; }
void Entity::ScaleSize(glm::vec3 _scale) { size *= _scale; }
void Entity::SetSize(glm::vec3 _size) { size = _size; }
void Entity::AddRot(float _addition) { rot += _addition; }
void Entity::SetRot(float _rot) { rot = _rot; }

void Entity::TransformLocalCoord (glm::vec3 &point) const {
  glm::mat4 transformation(1.0f);
  transformation = glm::translate(transformation, pos);
  transformation = glm::rotate(transformation, rot, glm::vec3(0.0f, 0.0f, 1.0f));
  transformation = glm::scale(transformation, size);
  glm::vec4 extraPoint(point, 1.0f);
  extraPoint = transformation * extraPoint;
  point = glm::vec3(extraPoint);
}

void Entity::TransformLocalCoord (glm::mat4 &body) const {
  body = glm::translate(body, pos);
  body = glm::rotate(body, rot, glm::vec3(0.0f, 0.0f, 1.0f));
  body = glm::scale(body, size);
}

void Entity::TransformToLocalCoord (glm::vec3 &point) const {
  glm::mat4 transformation(1.0f);
  glm::vec3 reciprocalSize(1 / size[0], 1 / size[1], 1 / size[2]);
  transformation = glm::scale(transformation, reciprocalSize);
  transformation = glm::rotate(transformation, -rot, glm::vec3(0.0f, 0.0f, 1.0f));
  transformation = glm::translate(transformation, pos * -1.0f);
  glm::vec4 extraPoint(point, 1.0f);
  extraPoint = transformation * extraPoint;
  point = glm::vec3(extraPoint);
}

float Entity::TransformLocalRot (float r) const {
  return r + rot;
}

void Entity::SetTransformation(glm::vec3 _pos, glm::vec3 _size, float _rot) {
  pos = _pos;
  size = _size;
  rot = _rot;
}
// :END ENTITY DECLARATION
// 
//
// BEGIN TEXTURED ENTITY DEFINITION:

TexturedEntity::TexturedEntity() {}
TexturedEntity::TexturedEntity(TexturedShader* r, TextureSheet *_texture) 
  : Entity(r),
    texture(_texture) {}
TexturedEntity::TexturedEntity(TexturedShader* r, TextureSheet *_texture, glm::vec3 _pos, glm::vec3 _size, float _rot)
  : Entity(r, _pos, _size, _rot),
    texture(_texture) {}

TexturedEntity::~TexturedEntity() {}

void TexturedEntity::Render(glm::mat4 modelMatrix, float* map, int points) const {
  Entity::Render(modelMatrix, texture->GetId(), map, points);
}

// Getter
TextureSheet *TexturedEntity::GetTexture() const {
  return texture;
}

// Setter
void TexturedEntity::SetTexture(TextureSheet *newTexture) {
  texture = newTexture;
}
// END TEXTURED ENTITY DECLARATION
//
//
// BEGIN UNTEXTURED ENTITY DECLARATION

UntexturedEntity::UntexturedEntity() {}
UntexturedEntity::UntexturedEntity(UntexturedShader* r, glm::vec3 _color)
  : Entity(r),
    color(_color) {}

UntexturedEntity::UntexturedEntity(UntexturedShader* r, glm::vec3 _color, glm::vec3 _pos, glm::vec3 _size, float _rot)
  : Entity(r, _pos, _size, _rot),
    color(_color) {}

UntexturedEntity::~UntexturedEntity() {}

void UntexturedEntity::Render(glm::mat4 modelMatrix, float* map, int points) const {
  GetProgram()->SetColor(color[0], color[1], color[2], 1.0);
  Entity::Render(modelMatrix, (GLuint)NULL, map, points);
}

glm::vec3 UntexturedEntity::GetColor() const {
  return color;
}

void UntexturedEntity::SetColor(glm::vec3 newColor) {
  color = newColor;
}
// :END UNTEXTURED ENTITY DECLARATION
//
// 
// BEGIN ENTITY GROUP DECLARATION
// ---DEFINED INLINE - SEE ENTITY.H---
// END ENTITY GROUP DECLARATION
//
Container::Container(std::initializer_list<Entity *> entities)
  : EntityGroup<Entity>(entities),
    Entity(NULL) 
{
  float maxLeft = INFINITY, maxRight = -INFINITY, maxUp = -INFINITY, maxDown = INFINITY;
  for (Entity *e : entities) {
    glm::mat4x3 c = e->GetCorners();

    // std::cout << c[0][0] << "," << c[1][0] << "," << c[0][1] << "," << c[3][1] << std::endl;
    if (c[0][0] < maxLeft) {
      maxLeft = c[0][0];
    }
    if (c[1][0] > maxRight) {
      maxRight = c[1][0];
    } 
    if (c[0][1] > maxUp) {
      maxUp = c[0][1];
    }
    if (c[3][1] < maxDown) {
      maxDown = c[3][1];
    }
  }

  // std::cout << "WIDTH" << maxRight - maxLeft << std::endl;
  // std::cout << "HEIGHT" << maxUp - maxDown << std::endl;
  SetSize(glm::vec3(maxRight - maxLeft, maxUp - maxDown, 1.0f));
  SetPos(glm::vec3(((maxRight - maxLeft) / 2) + maxLeft, (maxUp - maxDown) / 2 + maxDown, 1.0f));

  std::cout << "POS" << GetPos()[0] << "," << GetPos()[1] << std::endl;

} 

void Container::Render() {
  for (int i = 0; i < GetNumChildren(); ++i) {
    GetEntity(i)->Render();
  }
}

void Container::Update(float delta) {
  for (int i = 0; i < GetNumChildren(); ++i) {
    GetEntity(i)->Update(delta);
  }
}

// Adders
void Container::AddPos(glm::vec3 add) {
  Entity::AddPos(add);
  for (int i = 0; i < GetNumChildren(); ++i) {
    GetEntity(i)->AddPos(add);
  }
}