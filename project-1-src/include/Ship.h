#ifndef GAME_SHIP
#define GAME_SHIP

#define GL_SILENCE_DEPRECATION

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ShaderProgram.h>

class Ship {
 public : 
  glm::vec3 pos;
  float height;
  float width;
  float rotation;
  GLuint textureID;
  
  Ship();
  void Create (float _x, float _y, float _h, float _w, float r);
  void Display (ShaderProgram *program);
  void Destroy ();
  void Reposition (glm::vec3 *mov, float delta);
};

#endif
