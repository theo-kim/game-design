#include "../include/Scenes.h"
#include "../include/framework/ui/standard.h"
#include "../include/framework/animation/Blink.h"

//
// BEGIN OPENNINGSCENE DECLARATION :
OpenningScene::OpenningScene(glm::vec3 size) 
    : SimpleScene(size, glm::vec3(0.0f)) {}

Scene *OpenningScene::Update(float delta) {
    if (GetNumTransitions() > 0) {
        Scene *next = GetTransition(0);
        Unload();
        next->Load();
        return next;
    }
    return SimpleScene::Update(delta);
}

void OpenningScene::Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, const int mouseX, int mouseY) {
    if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym == SDLK_RETURN) {
            AddTransition(new MainMenuScene(GetDimensions()));
        }
    }
}

void OpenningScene::Load() {
    Scene::Load();

    AddEntity({
        new Image(*this, "textures/logo.png", glm::vec3(0.0f, 0.5f, 0.0f), 2.0f),
        new Blink(new Text("press enter to begin", *this, glm::vec3(0.0f, -0.5f, 0.0f), 0.1), 1)
    });
}
// : END OPENINGSCENE DECLARATION
//
//
// BEGIN MAIN MENU SCENE DECLARATION:
MainMenuScene::MainMenuScene(glm::vec3 size)
    : CompoundScene(size, glm::vec3(0.0f)) {}

Scene *MainMenuScene::Update(float delta) {
    CompoundScene::Update(delta);
    return NULL;
}

void MainMenuScene::Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, const int mouseX, int mouseY) {
    CompoundScene::Input(event, keys, mouse, mouseX, mouseY);
}

void MainMenuScene::Load() {
    MenuScene *menu = new MenuScene(GetDimensions(), glm::vec4(0.0f));
    SimpleScene *simple = new SimpleScene(GetDimensions(), glm::vec4(0.0f));
    AddLayer(glm::vec3(0.0f), menu);
    AddLayer(glm::vec3(0.0f), simple);
    CompoundScene::Load();

    menu->SetMenu(new Menu(
        *menu, 
        glm::vec3(-3.0f, 0.0f, 0.0f), 
        0.15, 
        0.10,
        Text::LEFT, 
        { "Michael Scott", "Dwight Schrute", "Jim Halpert", "Creed Bratton" }
    ));
    
    simple->AddEntity({
        new Blink(
            new Text(
                "Choose Your Character", *simple, glm::vec3(0.0f, 2.0f, 0.0f), 0.135
            ), 
            1.0f
        ),
        new Rect(*simple, glm::vec3(1.5f, 0.5f, 0.0f), glm::vec3(1.0f, 1.5f, 1.0f), 0.0f, glm::vec3(0.5f)),
        new Text("Micheal is a pretty nice boss,", *simple, glm::vec3(1.5f, -0.65f, 0.0f), 0.1),
        new Text("but don't upset him, otherwise,", *simple, glm::vec3(1.5f, -0.8f, 0.0f), 0.1),
        new Text("he'll turn into Prison Mike and,", *simple, glm::vec3(1.5f, -0.95f, 0.0f), 0.1),
        new Text("will scare you straight.", *simple, glm::vec3(1.5f, -1.1f, 0.0f), 0.1),
    });
}
// : END MAINMENUSCENE DECLARATION
// 