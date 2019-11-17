#include "../include/framework/ui/Image.h"

Image::Image(TexturedShader *program, const char *path, glm::vec3 pos, float width)
    : TexturedEntity(program, new TextureSheet(path, 1, 1)),
      Entity(program, pos, glm::vec3(1.0f), 0.0f)
{
    SetSize(glm::vec3(width, (width / GetTexture()->GetSize()), 1.0f));
}

// abstract implementations
void Image::Render() {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, GetPos());
    modelMatrix = glm::rotate(modelMatrix, GetRot(), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, GetSize()); 

    float map[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5,
            -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1 };

    GetTexture()->GetSprite(map + 12, 1, 1);

    TexturedEntity::Render(modelMatrix, map, 12);
}

void Image::Update(float delta) {
    return;
}