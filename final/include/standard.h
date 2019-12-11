#ifndef HEADERS_DEF
#define HEADERS_DEF

#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <math.h>
#include <vector>
#include <initializer_list>

#define GL3_PROTOTYPES 1

#include <GL/glew.h>
#include <SDL.h>
// #include <OpenGL/gl3.h>
// #include <OpenGL/gl3ext.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

// #ifdef __APPLE__
// #define glGenVertexArrays glGenVertexArraysAPPLE
// #define glBindVertexArray glBindVertexArrayAPPLE
// #define glDeleteVertexArrays glDeleteVertexArraysAPPLE
// #endif

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <LoadTexture.h>

class Large {
public:
  Large(float b, int e) : base(b), exp(e) { fix(); }
  float base;
  int exp;
  Large operator*(const Large& l) const { return Large(base * l.base, exp + l.exp); }
  Large operator/(const Large& l) const { return Large(base / l.base, exp - l.exp); }
  Large operator+(const Large& l) const {
    if (exp - l.exp > 5) return *this;
    if (l.exp - exp > 5) return l;
    if (l.exp > exp) return Large(base / ((l.exp - exp) * 10) + l.base, l.exp);
    return Large(l.base / ((exp - l.exp) * 10) + base, exp);
  }
  Large operator-(const Large& l) const {
    return *this + l * Large(-1, 0);
  }
  bool operator==(const Large& l) const {
    return base == l.base && exp == l.exp;
  }
  operator float() const {
    return base * pow(10.0f, exp);
  }
  void fix() {
    while (base >= 10.0f) {
      base /= 10.0f;
      exp++;
    }
  }
};

#define radians(x) x*(3.141592f/180.0f)

#endif
