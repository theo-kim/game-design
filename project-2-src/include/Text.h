#ifndef TEXT_DEF
#define TEXT_DEF

#include "standard.h"
#include <string>
#define CHAR_HEIGHT 42.0f
#define CHAR_WIDTH 26.0f
#define SHEET_HEIGHT 350.0f
#define SHEET_WIDTH 240.0f
#define NUM_ROW 9.0f
#define CHARSET " 0123456789?ABCDEFGHIJKLMNOPQRSTUVWXYZ[]abcdefghijklmnopqrstuvwxyz"

class Text {
 public:
  Text();
  Text(std::string _contents, ShaderProgram *_program, float x, float y, float width, float height, GLuint texture);
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

  GLuint textureID;

  std::string legalCharacters;
};

#endif
