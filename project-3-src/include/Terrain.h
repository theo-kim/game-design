#ifndef TERRAIN_DEF
#define TERRAIN_DEF

#include "standard.h"
#include <vector>

class Terrain {
 public:
  Terrain ();
  Terrain (ShaderProgram *_program, float left, float right, float bottom, float top, int seed);
  void Render ();
  void Update (float delta);
  glm::mat4x3 GetBounds (glm::vec3 segment);
  float GetMass ();
  void Generate(int segments, int minGradient, int maxGradient, int difficulty);
  void ZoomTo(float close, glm::vec3 focus);
private:
  // Properties
  float left;
  float right;
  float top;
  float bottom;
  
  // Rendering variables
  ShaderProgram *program;
  TextureSheet texture;
  int segments;
  std::vector<float> vertices;

  // Transformation Variables
  glm::vec3 pos;
  glm::vec3 zoom; // Scale for zooming in 
  glm::vec3 zoomShift; // translate to view center

  // Physics
  
};

#endif
