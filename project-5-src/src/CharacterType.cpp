#include "../include/CharacterTypes.h"

TextureSheet *Jim::Sprite = NULL;

Jim::Jim(TexturedShader *program, glm::vec3 pos)
    : Character(program, Jim::Sprite, NULL, NULL, 1, 1, 1),
      Entity(program, pos, glm::vec3(Jim::Sprite->GetSpriteSize() * 0.5f, 0.5, 1.0f), 0.0f)
{}

Jim::Jim(TexturedShader *program, glm::vec3 pos, float height)
    : Character(program, Jim::Sprite, NULL, NULL, 1, 1, 1),
      Entity(program, pos, glm::vec3(Jim::Sprite->GetSpriteSize() * height, height, 1.0f), 0.0f)
{}

TextureSheet *Dwight::Sprite = NULL;

Dwight::Dwight(TexturedShader *program, glm::vec3 pos)
    : Character(program, Dwight::Sprite, NULL, NULL, 1, 1, 1),
      Entity(program, pos, glm::vec3(Dwight::Sprite->GetSpriteSize() * 0.5f, 0.5, 1.0f), 0.0f)
{}

Dwight::Dwight(TexturedShader *program, glm::vec3 pos, float height)
    : Character(program, Dwight::Sprite, NULL, NULL, 1, 1, 1),
      Entity(program, pos, glm::vec3(Dwight::Sprite->GetSpriteSize() * height, height, 1.0f), 0.0f)
{}

TextureSheet *Michael::Sprite = NULL;

Michael::Michael(TexturedShader *program, glm::vec3 pos)
    : Character(program, Michael::Sprite, NULL, NULL, 1, 1, 1),
      Entity(program, pos, glm::vec3(Michael::Sprite->GetSpriteSize() * 0.5f, 0.5, 1.0f), 0.0f)
{}

Michael::Michael(TexturedShader *program, glm::vec3 pos, float height)
    : Character(program, Michael::Sprite, NULL, NULL, 1, 1, 1),
      Entity(program, pos, glm::vec3(Michael::Sprite->GetSpriteSize() * height, height, 1.0f), 0.0f)
{}


TextureSheet *Stanley::Sprite = NULL;

Stanley::Stanley(TexturedShader *program, glm::vec3 pos)
    : Character(program, Stanley::Sprite, NULL, NULL, 1, 1, 1),
      Entity(program, pos, glm::vec3(Stanley::Sprite->GetSpriteSize() * 0.5f, 0.5, 1.0f), 0.0f)
{}

Stanley::Stanley(TexturedShader *program, glm::vec3 pos, float height)
    : Character(program, Stanley::Sprite, NULL, NULL, 1, 1, 1),
      Entity(program, pos, glm::vec3(Stanley::Sprite->GetSpriteSize() * height, height, 1.0f), 0.0f)
{}