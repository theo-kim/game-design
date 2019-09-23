#include <stdlib.h>
#include <iostream>
#include "include/Ship.h"

Ship::Ship() {}

void Ship::Create(float x, float y, float w, float h, float r) {
  pos = glm::vec3(x, y, 1.0f);

  height = h;
  width = w;
  rotation = r;
}

void Ship::Display(ShaderProgram *program) {
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
  modelMatrix = glm::translate(modelMatrix, pos);
  modelMatrix = glm::scale(modelMatrix, glm::vec3(width, height, 0.0f));
  program->SetModelMatrix(modelMatrix);
  
  float vertices[]  = { -0.4, -0.9, 0.5, -0.9, 0.5, 0.5, -0.4, -0.9, 0.5, 0.5, -0.4, 0.5 };
  float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
  glBindTexture(GL_TEXTURE_2D, textureID);
    
  glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
  glEnableVertexAttribArray(program->positionAttribute);
    
  glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
  glEnableVertexAttribArray(program->texCoordAttribute);
    
  glDrawArrays(GL_TRIANGLES, 0, 6);
    
  glDisableVertexAttribArray(program->positionAttribute);
  glDisableVertexAttribArray(program->texCoordAttribute);
}

void Ship::Destroy() { }

void Ship::Reposition (glm::vec3 *mov, float delta) {
  pos += *mov * delta;
  if (pos[0] > 5) {
    pos[0] = -5.0f;
    pos[1] = (float)(rand() % 200 - 100) / 25;
  }
}
