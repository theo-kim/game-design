#include "include/Text.h"

TextureSheet *Text::fontSheet = new TextureSheet("textures/font-sheet-bold.png", NUM_ROW, NUM_COL);

Text::Text() {}

Text::Text(std::string _contents, ShaderProgram *_program, float x, float y, float width, float height)
  : Entity(glm::vec3(x, y, 0.0f), glm::vec3(width * _contents.size(), height, 1.0f), 0.0f, fontSheet, _program)
{
  contents = _contents;
  legalCharacters = CHARSET;
  charSize = glm::vec3(width, height, 1.0f);
}

void Text::Render() {
  int totalChars = contents.size();
  
  // Render each character individually
  for (int i = 0; i < contents.size(); ++i) {
    float index = i - ((float)(totalChars - 1) / 2);
    glm::vec3 charOffset = glm::vec3(charSize[0] * index, 0.0f, 0.0f);
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, pos + charOffset);
    modelMatrix = glm::scale(modelMatrix, charSize);

    float map[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5,
		     0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

    GetCharMap(map + 12, contents[i]);

    Entity::Render(modelMatrix, fontSheet->GetId(), map, 12);
  }
}

void Text::Update(float delta) {}

void Text::SetText (std::string newtext) {
  contents = newtext;
}

std::string Text::GetText() {
  return contents;
}

void Text::GetCharMap (float *coords, char c) {
  // LB, RB, RT, LB, RT, LT
  int spriteIndex = 0;
  for (spriteIndex = 0; spriteIndex < legalCharacters.size(); ++spriteIndex) {
    if (c == legalCharacters[spriteIndex]) {
      break;
    }
  }

  if (spriteIndex == legalCharacters.size()) {
    std::cout << "ILLEGAL CHAR" << std::endl;
  }

  int col = spriteIndex % (int)NUM_COL;
  int row = spriteIndex / (int)NUM_COL;

  fontSheet->GetSprite(coords, col, row);
}
