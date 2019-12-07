#ifndef LOAD_TEXTURE_INCLUDE
#define LOAD_TEXTURE_INCLUDE

#define GL_SILENCE_DEPRECATION

#include <SDL.h>
// #include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>

class TextureSheet {
public:
  TextureSheet();
  TextureSheet(const char* filePath, int numRows, int numCols);
  GLuint GetId();
  float GetSize();
  float GetSpriteSize();
  void GetSprite (float *coords, int x, int y);
private:
  GLuint LoadTexture (const char* filePath);
  GLuint id;
  float sheetHeight;
  float sheetWidth;
  float spriteWidth;
  float spriteHeight;
};

#endif
