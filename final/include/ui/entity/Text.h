#ifndef TEXT_DEF

#include "Entity.h"
#include "standard.h"

class Text : public Entity {
public:
  Text(const char* fontName, float fontSize, glm::vec3 color, Transformation *transformation, ShaderProgram *sp);
  virtual void Render() const;
  virtual void SetText(std::string text);
private:
  std::string contents;
  glm::vec3 color;
  float size;
  TTF_Font *gFont;
  GLuint textureId;
  SDL_Surface *surface;
};

#endif