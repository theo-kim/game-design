#include "../include/Scenes.h"
#include "../include/framework/ui/standard.h"
#include "../include/framework/animation/Blink.h"
#include "../include/Character.h"
#include "../include/CharacterTypes.h"

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

void OpenningScene::Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, float mouseX, float mouseY) {
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
    : CompoundScene(size, glm::vec3(0.3f)) {}

Scene *MainMenuScene::Update(float delta) {
    CompoundScene::Update(delta);

    int index = menu->GetSelectedIndex();
    if (index != -1) {
        Level1 *next = new Level1(GetDimensions(), glm::vec3(50.0f, 50.0f, 1.0f));
        Unload();
        next->Load();
        if (index == 0) {
            next->ChooseCharacter(new Michael(*next, glm::vec3(0.0f)));
        }
        else if (index == 1) {
            next->ChooseCharacter(new Stanley(*next, glm::vec3(0.0f)));
        }
        else if (index == 2) {
            next->ChooseCharacter(new Dwight(*next, glm::vec3(0.0f)));
        }
        else {
            next->ChooseCharacter(new Jim(*next, glm::vec3(0.0f)));
        }
        return next;
    }
    return NULL;
}

void MainMenuScene::Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, float mouseX, float mouseY) {
    CompoundScene::Input(event, keys, mouse, mouseX, mouseY);
}

void MainMenuScene::Load() {
    menu = new MenuScene(GetDimensions(), glm::vec4(0.0f));
    simple = new SimpleScene(GetDimensions(), glm::vec4(0.0f));
    AddLayer(glm::vec3(0.0f), menu);
    AddLayer(glm::vec3(0.0f), simple);
    CompoundScene::Load();

    menu->SetMenu(new Menu({
        new AnimatedButton(
            new Slide(NULL, 0.2f, glm::vec3(0.0f, 0.2f, 0.0f), false),
            NULL,
            new Container({
                new Michael(*simple, glm::vec3(-1.5f, 0.0f, 0.0f), 1.0f),
                new Text("Micheal", *simple, glm::vec3(-1.5f, -0.7f, 0.0f), 0.1),
                new Text("Scott", *simple, glm::vec3(-1.5f, -0.85f, 0.0f), 0.1)
            })
        ),
        new AnimatedButton(
            new Slide(NULL, 0.2f, glm::vec3(0.0f, 0.2f, 0.0f), false),
            NULL,
            new Container({
                new Stanley(*simple, glm::vec3(-0.5f, 0.0f, 0.0f), 1.0f),
                new Text("Stanley", *simple, glm::vec3(-0.5f, -0.7f, 0.0f), 0.1),
                new Text("Hudson", *simple, glm::vec3(-0.5f, -0.85f, 0.0f), 0.1)
            })
        ),
        new AnimatedButton(
            new Slide(NULL, 0.2f, glm::vec3(0.0f, 0.2f, 0.0f), false),
            NULL,
            new Container({
                new Dwight(*simple, glm::vec3(0.5f, 0.0f, 0.0f), 1.0f),
                new Text("Dwight", *simple, glm::vec3(0.5f, -0.7f, 0.0f), 0.1),
                new Text("Schrute", *simple, glm::vec3(0.5f, -0.85f, 0.0f), 0.1)
            })
        ),
        new AnimatedButton(
            new Slide(NULL, 0.2f, glm::vec3(0.0f, 0.2f, 0.0f), false),
            NULL,
            new Container({
                new Jim(*simple, glm::vec3(1.5f, 0.0f, 0.0f), 1.0f),
                new Text("Jim", *simple, glm::vec3(1.5f, -0.7f, 0.0f), 0.1),
                new Text("Halpert", *simple, glm::vec3(1.5f, -0.85f, 0.0f), 0.1)
            })
        ),
    }));

    simple->AddEntity({
        new Blink(
            new Text(
                "Choose Your Character", *simple, glm::vec3(0.0f, 2.0f, 0.0f), 0.135
            ), 
            1.0f
        )
    });
}
// : END MAINMENUSCENE DECLARATION
// 
//
//
Level1::Level1(glm::vec3 size, glm::vec3 maxSize)
    : CompoundScene(size, glm::vec3(0.3f)) {}

Scene *Level1::Update(float delta) {
    CompoundScene::Update(delta);
}

void Level1::Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, float mouseX, float mouseY) {
    CompoundScene::Input(event, keys, mouse, mouseX, mouseY);
}

void Level1::Load() {
    info = new SimpleScene(GetDimensions(), glm::vec4(0.0f));
    AddLayer(glm::vec3(0.0f), info);
    CompoundScene::Load();

    info->AddEntity({
        new Text("Level 1", *info, glm::vec3(-(GetDimensions()[0] / 2.5) + 0.1, (GetDimensions()[1] / 2.5) - 0.1, 0.0f), 0.1, Text::LEFT)
    });
}

void Level1::ChooseCharacter(Character *character) {
    player = character;
}
//
//