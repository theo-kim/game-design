#ifndef CHARACTERTYPE_DEF
#define CHARACTERTYPE_DEF

#include "standard.h"
#include "Character.h"

class Jim : public Character {
public:
    static TextureSheet *Sprite;
    Jim(TexturedShader *program, glm::vec3 pos);
    Jim(TexturedShader *program, glm::vec3 pos, float height);
};

class Dwight : public Character {
public:
    static TextureSheet *Sprite;
    Dwight(TexturedShader *program, glm::vec3 pos);
    Dwight(TexturedShader *program, glm::vec3 pos, float height);
};

class Michael : public Character {
public:
    static TextureSheet *Sprite;
    Michael(TexturedShader *program, glm::vec3 pos);
    Michael(TexturedShader *program, glm::vec3 pos, float height);
};

class Stanley : public Character {
public:
    static TextureSheet *Sprite;
    Stanley(TexturedShader *program, glm::vec3 pos);
    Stanley(TexturedShader *program, glm::vec3 pos, float height);
};

#endif