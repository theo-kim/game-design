#include <stdlib.h>
#include <iostream>
#include "include/Star.h"

Star::Star() {}

void Star::Create(unsigned int seed) {
  srand(seed);
  
  float x = ((float)(std::rand() % 200 - 100) / 20);
  float y = ((float)(std::rand() % 200 - 100) / 25);

  pos = glm::vec3(x, y, 0.0f);

  height = ((float)(std::rand() % 85 + 50) / 1000);
  width = height;
}

void Star::Display(ShaderProgram *program) {
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, pos);
  modelMatrix = glm::scale(modelMatrix, glm::vec3(width, height, 0.0f));
  
  
  
  float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
  float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

  glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
  glEnableVertexAttribArray(program->positionAttribute);
    
  glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
  glEnableVertexAttribArray(program->texCoordAttribute);
    
  glBindTexture(GL_TEXTURE_2D, (GLuint)NULL);
  program->SetModelMatrix(modelMatrix);
  glDrawArrays(GL_TRIANGLES, 0, 6);
    
  glDisableVertexAttribArray(program->positionAttribute);
  glDisableVertexAttribArray(program->texCoordAttribute);
}

void Star::Destroy() { }

void Star::Reposition (glm::vec3 *mov, float delta) {
  pos += *mov * delta;
  if (pos[0] > 5) {
    pos[0] = -5.0f;
    pos[1] = (float)(rand() % 200 - 100) / 25;
  }
}
