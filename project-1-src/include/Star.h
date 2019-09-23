#ifndef GAME_STAR
#define GAME_STAR

#define GL_SILENCE_DEPRECATION

#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ShaderProgram.h>

class Star {
 public : 
  glm::vec3 pos;
  float height;
  float width;
  
  Star();
  void Create (unsigned int seed);
  void Display (ShaderProgram *program);
  void Destroy ();
  void Reposition (glm::vec3 *mov, float delta);
};

#endif
