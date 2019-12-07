#include "LoadTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureSheet::TextureSheet() {}

TextureSheet::TextureSheet(const char *filepath, int numRows, int numCols) {
  id = LoadTexture(filepath);
  spriteWidth = sheetWidth / numCols;
  spriteHeight = sheetHeight / numRows;
}

GLuint TextureSheet::LoadTexture (const char* filePath) {
  int w, h, n;
  unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

  if (image == NULL) {
    std::cout << "Unable to load image. Make sure the path is correct\n";
    assert(false);
  }

  sheetHeight = (float)h;
  sheetWidth = (float)w;
  
  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  stbi_image_free(image);
  return textureID;
}

GLuint TextureSheet::GetId() {
  return id;
}

float TextureSheet::GetSize() {
  return sheetWidth / sheetHeight;
}

float TextureSheet::GetSpriteSize() {
  return spriteWidth / spriteHeight;
}

void TextureSheet::GetSprite(float coords[], int x, int y) {
  float scaledWidth = spriteWidth / sheetWidth;
  float scaledHeight = spriteHeight / sheetHeight;

  float top = scaledHeight * y;
  float left = scaledWidth * x;
  float right = left + scaledWidth;
  float bottom = top + scaledHeight;

  coords[0] = left;
  coords[1] = bottom;;
  coords[2] = right;
  coords[3] = bottom;
  coords[4] = right;
  coords[5] = top;
  coords[6] = left;
  coords[7] = bottom;
  coords[8] = right;
  coords[9] = top;
  coords[10] = left;
  coords[11] = top;
}
