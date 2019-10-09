#define SEGMENT_PADDING 30
#include "include/Terrain.h"

Terrain::Terrain() { }

Terrain::Terrain(ShaderProgram *_program, float _left, float _right, float _bottom, float _top, int seed) {
  program = _program;
  left = _left;
  right = _right;
  bottom = _bottom;
  top = _top;
  pos = glm::vec3(0.0f);
  zoom = glm::vec3(1.0f);
  zoomShift = glm::vec3(0.0f);
  srand(seed);
}

void Terrain::Render() {
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  // Position and size the model matrix
  modelMatrix = glm::translate(modelMatrix, pos);
  modelMatrix = glm::scale(modelMatrix, zoom);
  modelMatrix = glm::translate(modelMatrix, zoomShift);
  program->SetModelMatrix(modelMatrix);

  glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
  glEnableVertexAttribArray(program->positionAttribute);

  glBindTexture(GL_TEXTURE_2D, (GLuint)NULL);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2.0f);

  glDisableVertexAttribArray(program->positionAttribute);
}

void Terrain::Generate(int _segments, int minGradient, int maxGradient, int difficulty) {
  segments = _segments + SEGMENT_PADDING;
  float segmentWidth = (right - left) / (float)(segments - SEGMENT_PADDING);
  int gradientRange = maxGradient - minGradient;
  float lastY = top;
  float lastX = left - segmentWidth * (SEGMENT_PADDING / 2);
  vertices.clear();
  for (int i = 0; i < segments; ++i) {
    float gradient = 0;
    if (rand() % 100 > difficulty) {
      gradient = (float)(rand() % (gradientRange * 100) + (minGradient * 100)) / 100.0f;
    }
    if (gradient < 0.1f && gradient > -0.1f) gradient = 0;
    float newX = lastX + segmentWidth;
    float newY = gradient * (newX - lastX) + lastY;
    if (newY < bottom * 0.95) {
      newY = bottom * 0.95;
    }
    
    // First triangle
    vertices.push_back(lastX);
    vertices.push_back(lastY);
    vertices.push_back(lastX);
    vertices.push_back(bottom * 2);
    vertices.push_back(newX);
    vertices.push_back(bottom * 2);
    // Second triangle
    vertices.push_back(lastX);
    vertices.push_back(lastY);
    vertices.push_back(newX);
    vertices.push_back(bottom * 2);
    vertices.push_back(newX);
    vertices.push_back(newY);

    lastX = newX;
    lastY = newY;
  }
}

void Terrain::ZoomTo(float close, glm::vec3 focus) {
  zoom = glm::vec3(close, close, 1.0f);
  zoomShift = focus;
}

// Physics
glm::mat4x3 Terrain::GetBounds(glm::vec3 seg) {
  float segWidth = (right - left) / (float)(segments - SEGMENT_PADDING);
  int segIndex = floor(seg[0] / segWidth) + (segments / 2);
  
  int vectorIndex = segIndex * 12;
  
  glm::vec3 topLeft = glm::vec3(vertices[vectorIndex], vertices[vectorIndex + 1], 1.0f);
  glm::vec3 bottomLeft = glm::vec3(vertices[vectorIndex], vertices[vectorIndex + 3], 1.0f);
  glm::vec3 topRight = glm::vec3(vertices[vectorIndex + 4], vertices[vectorIndex + 11], 1.0f);
  glm::vec3 bottomRight = glm::vec3(vertices[vectorIndex + 4], vertices[vectorIndex + 3], 1.0f);

  return glm::mat4x3(topLeft, bottomLeft, topRight, bottomRight);
}
