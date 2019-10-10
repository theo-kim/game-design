#include "include/Text.h"

Text::Text() {}

Text::Text(std::string _contents, ShaderProgram *_program, float x, float y, float width, float height, const char *filename) {
  contents = _contents;
  program = _program;
  pos = glm::vec3(x, y, 0.0f);
  size = glm::vec3(width, height, 1.0f);
  texture = TextureSheet(filename, NUM_ROW, NUM_COL);

  legalCharacters = CHARSET;
}

void Text::Render() {
  int totalChars = contents.size();
  
  // Render each character individually
  for (int i = 0; i < contents.size(); ++i) {
    float index = i - ((float)(totalChars - 1) / 2);
    glm::vec3 charOffset = glm::vec3(size[0] * index, 0.0f, 0.0f);
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, pos + charOffset);
    modelMatrix = glm::scale(modelMatrix, size);
    program->SetModelMatrix(modelMatrix);

    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[12];

    GetCharMap(texCoords, contents[i]);

    glBindTexture(GL_TEXTURE_2D, texture.GetId());

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
  }
}

void Text::SetText (std::string newtext) {
  contents = newtext;
}

std::string Text::GetText() {
  return contents;
}

void Text::GetCharMap (float *coords, char c) {
  // LB, RB, RT, LB, RT, LT
  int spriteIndex = 0;
  for (spriteIndex  = 0; spriteIndex < legalCharacters.size(); ++spriteIndex) {
    if (c == legalCharacters[spriteIndex]) {
      break;
    }
  }

  int col = spriteIndex % (int)NUM_COL;
  int row = spriteIndex / (int)NUM_COL;

  texture.GetSprite(coords, col, row);
}
