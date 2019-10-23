#include "include/Star.h"

Star::Star() {}

Star::Star(ShaderProgram *_program, unsigned int seed, float left, float right, float top, float bottom) {
  srand(seed);

  int rangeh = (int)((std::fabs(left) + std::fabs(right)) * 200.0f);
  int rangev = (int)((std::fabs(top) + std::fabs(bottom)) * 200.0f);
  float x = ((float)(std::rand() % rangeh + (int)(left * 100.0f)) / 100.0f);
  float y = ((float)(std::rand() % rangev + (int)(left * 100.0f)) / 100.0f);
  
  pos = glm::vec3(x, y, 0.0f);

  float height = ((float)(std::rand() % 40 + 20) / 1000);
  float width = height;

  size = glm::vec3(width, height, 1.0f);

  program = _program;
}

void Star::Render() {
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, pos);
  modelMatrix = glm::scale(modelMatrix, size);
  
  float map[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5,
		   0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

  program->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

  Entity::Render(modelMatrix, (GLuint)NULL, map, 12);
}

void Star::Update(float delta) {
  return;
}
