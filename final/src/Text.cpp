#include "ui/entity/Text.h"
#include "ui/mesh/Mesh2D.h"

Text::Text(const char* fontName, float fontSize, glm::vec3 fontColor, Transformation *transformation, ShaderProgram *sp) 
  : Entity(new Mesh2D(
            { 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, 0.5 },
            { 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0 }
          ), transformation, sp),
    size(fontSize),
    color(fontColor)
{
  gFont = TTF_OpenFont(fontName, fontSize);
}

void Text::SetText(std::string text) {
  SDL_Color sColor = { (Uint8)(color[0] * 255), (Uint8)(color[1] * 255), (Uint8)(color[2] * 255) };
  surface = TTF_RenderText_Blended(gFont, text.c_str(), sColor);
  
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  int Mode = GL_RGB;
 
  if(surface->format->BytesPerPixel == 4) {
      Mode = GL_RGBA;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, Mode, surface->w, surface->h, 0, 
    Mode, GL_UNSIGNED_BYTE, surface->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  transformation->scale = Scale(transformation->scale.vector[1] * surface->w / surface->h, transformation->scale.vector[1], 1.0f);
}

void Text::Render() const {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureId);
  Entity::Render();
}