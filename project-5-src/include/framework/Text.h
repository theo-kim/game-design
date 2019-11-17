#ifndef TEXT_DEF
#define TEXT_DEF

#include "standard.h"
#include "Entity.h"
#include <string>

#define CHARSET " !\"#$%&'()*+,-./0123456789:;=?ABCDEFGHIJKLMNOPQRSTUVWXYZ^_`abcdefghijklmnopqrstuvwxyz"
#define NUM_ROW 7
#define NUM_COL 13

class Text: public TexturedEntity {
public:
  enum Alignment { CENTER, LEFT, RIGHT };
  
  Text();
  Text(std::string _contents, TexturedShader *_program, float x, float y, float width, float height);
  Text(std::string _contents, TexturedShader *_program, float x, float y, float width, float height, Alignment alignment);
  virtual void Render();
  virtual void Update(float delta); 
  void SetText(std::string newContents);
  std::string GetText();

  static TextureSheet *fontSheet;
private:
  bool CheckLegalChar(char c);
  void GetCharMap(float *coords, char c);
  
  std::string contents;
  
  glm::vec3 charSize;

  std::string legalCharacters;

  Alignment align;
};

#endif
