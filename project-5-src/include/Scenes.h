#ifndef GAME_SCENES_DEF
#define GAME_SCENES_DEF

#include "framework/standard.h"
#include "Character.h"
#include "standard.h"
#include "CharacterTypes.h"

class OpenningScene : public SimpleScene {
public:
    OpenningScene(glm::vec3 size);
    
    virtual Scene *Update(float delta);
    virtual void Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, float mouseX, float mouseY);
    virtual void Load();
};

class MainMenuScene : public CompoundScene {
public:
    MainMenuScene(glm::vec3 size);

    virtual Scene *Update(float delta);
    virtual void Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, float mouseX, float mouseY);
    virtual void Load();
private:
    SimpleScene *simple;
    MenuScene *menu;
};

class Level1 : public CompoundScene, public ActionScene {
public:
    Level1(glm::vec3 size, glm::vec3 maxSize);

    virtual Scene *Update(float delta);
    virtual void Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, float mouseX, float mouseY);
    virtual void Load();
    virtual void Render();

    void ChooseCharacter(Character *character);
private:
    Character *player;
    Text *timer;
    Text *lives;
    Text *levelLabel;

    Jan *jan;
    Robert *robert;
    int numlives;
    int currentLevel;
    
    float ulttimer;

    SimpleScene *info;
    SimpleScene *action;


    std::vector<std::initializer_list<Entity *> > levels;
};

#endif