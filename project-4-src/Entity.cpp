#include "include/Entity.h"

Entity::Entity() {};

Entity::Entity(glm::vec3 _pos, glm::vec3 _size, float _rot, ShaderProgram *_program)
  :pos(_pos), size(_size), rot(_rot), program(_program), doRender(true)
{
  
}

Entity::Entity(glm::vec3 _pos, glm::vec3 _size, float _rot, TextureSheet *_texture, ShaderProgram *_program)
  :pos(_pos), size(_size), rot(_rot), program(_program), doRender(true),
   texture(_texture)
{
  
}

Entity::~Entity() {}

void Entity::Render(glm::mat4 modelMatrix, GLuint texture, float* map, int points) {
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
  return pos[0] + size[0] / 2  > left
    && pos[0] - size[0] / 2 < right
    && pos[1] + size[1] / 2 > bottom
    && pos[1] - size[1] / 2 < top;
}

bool Entity::CheckBounds(float left, float right, float top, float bottom) {
  return pos[0] - size[0] / 2  > left
    && pos[0] + size[0] / 2 < right
    && pos[1] - size[1] / 2 > bottom
    && pos[1] + size[1] / 2 < top;
}

glm::vec3 Entity::GetPos () const {
  return pos;
}

glm::mat4 Entity::GetCorners () const {
  float left = pos[0] - size[0] / 2;
  float right = pos[0] + size[0] / 2;
  float top =  pos[1] + size[1] / 2;
  float bottom = pos[1] - size[1] / 2;
  glm::vec4 topLeft = glm::vec4(left, top, 1.0f, 1.0f);
  glm::vec4 topRight = glm::vec4(right, top, 1.0f, 1.0f);
  glm::vec4 bottomLeft = glm::vec4(left, bottom, 1.0f, 1.0f);
  glm::vec4 bottomRight = glm::vec4(right, bottom, 1.0f, 1.0f);

  return glm::mat4(topLeft, topRight, bottomLeft, bottomRight);
}

void Entity::TransformLocalCoord (glm::vec3 &point) const {
  glm::mat4 transformation = glm::mat4(1.0f);
  transformation = glm::translate(transformation, pos);
  transformation = glm::rotate(transformation, -rot, glm::vec3(0.0f, 0.0f, 1.0f));
  glm::vec4 extraPoint = glm::vec4(point[0], point[1], point[2], 1.0f);
  glm::vec4 transformedPoint = extraPoint * transformation;
  point[0] = transformedPoint[0];
  point[1] = transformedPoint[1];
  point[2] = transformedPoint[2];
}

void Entity::TransformLocalCoord (glm::mat4 &body) const {
  body = glm::translate(body, pos);
  body = glm::rotate(body, -rot, glm::vec3(0.0f, 0.0f, 1.0f));
}
