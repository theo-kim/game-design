#ifndef TEXT_DEF
#define TEXT_DEF

#include "standard.h"
#include <string>

#define CHARSET " 0123456789?ABCDEFGHIJKLMNOPQRSTUVWXYZ[]abcdefghijklmnopqrstuvwxyz<>"
#define NUM_ROW 5
#define NUM_COL 14

class Text {
 public:
  Text();
  Text(std::string _contents, ShaderProgram *_program, float x, float y, float width, float height, const char *filename);
  void Render();
  void SetText(std::string newContents);
  std::string GetText();
 private:
  bool CheckLegalChar(char c);
  void GetCharMap(float *coords, char c);
  
  std::string contents;
  ShaderProgram *program;

  glm::vec3 pos;
  glm::vec3 size;

  TextureSheet texture;;

  std::string legalCharacters;
};

#endif
