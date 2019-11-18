#ifndef GAME_SCENES_DEF
#define GAME_SCENES_DEF

#include "framework/standard.h"
#include "standard.h"

class OpenningScene : public SimpleScene {
public:
    OpenningScene(glm::vec3 size);
    
    virtual Scene *Update(float delta);
    virtual void Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, const int mouseX, int mouseY);
    virtual void Load();
};

class MainMenuScene : public CompoundScene {
public:
    MainMenuScene(glm::vec3 size);

    virtual Scene *Update(float delta);
    virtual void Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, const int mouseX, int mouseY);
    virtual void Load();
};

#endif