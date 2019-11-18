#ifndef HEADERS_DEF
#define HEADERS_DEF

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <iostream>
#include <math.h>
#include <vector>
#include <initializer_list>

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/component_wise.hpp>
#include <ShaderProgram.h>
#include <LoadTexture.h>

#define radians(x) x*(3.141592f/180.0f)

#endif