#include "include/Text.h"

Text::Text() {}

Text::Text(std::string _contents, ShaderProgram *_program, float x, float y, float width, float height, GLuint texture) {
  contents = _contents;
  program = _program;
  pos = glm::vec3(x, y, 0.0f);
  size = glm::vec3(width, height, 1.0f);
  textureID = texture;

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

    glBindTexture(GL_TEXTURE_2D, textureID);

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

void Text::GetCharMap (float *coords, char c) {
  // LB, RB, RT, LB, RT, LT
  int spriteIndex = 0;
  for (spriteIndex  = 0; spriteIndex < legalCharacters.size(); ++spriteIndex) {
    if (c == legalCharacters[spriteIndex]) {
      break;
    }
  }

  int col = spriteIndex % (int)NUM_ROW;
  int row = spriteIndex / (int)NUM_ROW;

  float scaledWidth = CHAR_WIDTH / SHEET_WIDTH;
  float scaledHeight = CHAR_HEIGHT / SHEET_HEIGHT;

  float top = scaledHeight * row;
  float left = scaledWidth * col;
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
