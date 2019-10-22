#include "include/Entity.h"

Entity::Entity() {};

Entity::Entity(glm::vec3 _pos, glm::vec3 _size, float _rot, ShaderProgram *_program)
  :pos(_pos), size(_size), rot(_rot), program(_program)
{
  
}

void Entity::Render(glm::mat4 modelMatrix, GLuint texture, float* map, int points) {
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
