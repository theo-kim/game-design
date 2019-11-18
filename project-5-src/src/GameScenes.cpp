#include "../include/Scenes.h"
#include "../include/framework/ui/standard.h"
#include "../include/framework/animation/Blink.h"
#include "../include/Character.h"
#include "../include/CharacterTypes.h"
#include "../include/Platform.h"




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
    : CompoundScene(size, glm::vec3(0.3f)),
      Scene(size, glm::vec3(0.3f)) {}

Scene *MainMenuScene::Update(float delta) {
    CompoundScene::Update(delta);

    int index = menu->GetSelectedIndex();
    if (index != -1) {
        Level1 *next = new Level1(GetDimensions(), glm::vec3(50.0f, 50.0f, 1.0f));
        Unload();
        next->Load();
        if (index == 0) {
            next->ChooseCharacter(new Michael(*(next->GetLayer(1)), *next, *next, glm::vec3(0.0f), 0.25));
        }
        else if (index == 1) {
            next->ChooseCharacter(new Stanley(*(next->GetLayer(1)), *next, *next, glm::vec3(0.0f), 0.25));
        }
        else if (index == 2) {
            next->ChooseCharacter(new Dwight(*(next->GetLayer(1)), *(next->GetLayer(1)), *next, *next, glm::vec3(0.0f), 0.25));
        }
        else {
            next->ChooseCharacter(new Jim(*(next->GetLayer(1)), *next, *next, glm::vec3(0.0f), 0.25));
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
                new Michael(*menu, glm::vec3(-1.5f, 0.0f, 0.0f), 1.0f),
                new Text("Micheal", *menu, glm::vec3(-1.5f, -0.7f, 0.0f), 0.1),
                new Text("Scott", *menu, glm::vec3(-1.5f, -0.85f, 0.0f), 0.1)
            })
        ),
        new AnimatedButton(
            new Slide(NULL, 0.2f, glm::vec3(0.0f, 0.2f, 0.0f), false),
            NULL,
            new Container({
                new Stanley(*menu, glm::vec3(-0.5f, 0.0f, 0.0f), 1.0f),
                new Text("Stanley", *menu, glm::vec3(-0.5f, -0.7f, 0.0f), 0.1),
                new Text("Hudson", *menu, glm::vec3(-0.5f, -0.85f, 0.0f), 0.1)
            })
        ),
        new AnimatedButton(
            new Slide(NULL, 0.2f, glm::vec3(0.0f, 0.2f, 0.0f), false),
            NULL,
            new Container({
                new Dwight(*menu, glm::vec3(0.5f, 0.0f, 0.0f), 1.0f),
                new Text("Dwight", *menu, glm::vec3(0.5f, -0.7f, 0.0f), 0.1),
                new Text("Schrute", *menu, glm::vec3(0.5f, -0.85f, 0.0f), 0.1)
            })
        ),
        new AnimatedButton(
            new Slide(NULL, 0.2f, glm::vec3(0.0f, 0.2f, 0.0f), false),
            NULL,
            new Container({
                new Jim(*menu, glm::vec3(1.5f, 0.0f, 0.0f), 1.0f),
                new Text("Jim", *menu, glm::vec3(1.5f, -0.7f, 0.0f), 0.1),
                new Text("Halpert", *menu, glm::vec3(1.5f, -0.85f, 0.0f), 0.1)
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
    : CompoundScene(size, glm::vec3(0.3f)),
      ActionScene(maxSize[1], maxSize[0], size[0], size[1], size[2]),
      Scene(size, glm::vec3(0.3f))
{
    ulttimer = 0.0f;
}

Scene *Level1::Update(float delta) {
    ActionScene::Update(delta);
    CompoundScene::Update(delta);

    ulttimer += delta;
    if (ulttimer >= player->threshold && !player->ulting) ulttimer = player->threshold;
    else if (ulttimer >= player->threshold && player->ulting) ulttimer = 0;
    timer->SetText("Next Ability in " + std::to_string((int)(player->threshold - ulttimer)));
    lives->SetText("Lives Remaining: " + std::to_string(numlives));

    if (player->currentHealth <= 0) {
        action->AddEntity({
            new Text("Game Over!", *action, glm::vec3(0.0f), 0.2),
        });
    }
    if (jan->currentHealth <= 0) {
         action->AddEntity({
            new Text("You will!", *action, glm::vec3(0.0f), 0.2),
        });
    }
    return NULL;
}

void Level1::Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, float mouseX, float mouseY) {
    player->Walk(keys[SDL_SCANCODE_D] - keys[SDL_SCANCODE_A]);
    player->Jump(keys[SDL_SCANCODE_SPACE]);
    player->Ult(ulttimer >= player->threshold && keys[SDL_SCANCODE_Q]);
    // CompoundScene::Input(event, keys, mouse, mouseX, mouseY);
}

void Level1::Load() {
    numlives = 3;
    info = new SimpleScene(GetDimensions(), glm::vec4(0.0f));
    action = new SimpleScene(GetDimensions(), glm::vec4(0.0f));
    AddLayer(glm::vec3(0.0f), info);
    AddLayer(glm::vec3(0.0f), action);
    CompoundScene::Load();

    timer = new Text("Next Ability in X", *info, glm::vec3(-(GetDimensions()[0] / 2.5) + 0.1, (GetDimensions()[1] / 2.5) - 0.25f, 0.0f), 0.1, Text::LEFT);
    lives = new Text("Lives Remaining: X", *info, glm::vec3(-(GetDimensions()[0] / 2.5) + 0.1, (GetDimensions()[1] / 2.5) - 0.40f, 0.0f), 0.1, Text::LEFT);
    info->AddEntity({
        new Text("Level 1", *info, glm::vec3(-(GetDimensions()[0] / 2.5) + 0.1, (GetDimensions()[1] / 2.5) - 0.1, 0.0f), 0.1, Text::LEFT),
        timer,
        lives,
    });
    jan = new Jan(*action, *this, *this, glm::vec3(0.0f, -1.0f, 0.0f), 0.25f);
    action->AddEntity({
        //Level 1
        // new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(1.0f, -1.5f, 1.0f), 3), //base platform
        // new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(0.0f, -0.3f, 1.0f), 0.75), //spawn in middle
        // new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(1.1f, 0.12f, 1.0f), 1), //Step 1 right
        // new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(-1.1f, 0.12f, 1.0f), 1), //Step 1 left
        // new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(-2.2f, -0.6f, 1.0f), 1), //Step left 1
        // new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(-1.6f, -1.3f, 1.0f), 1), //Step left 2
        // new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(-3.0f, -1.7f, 1.0f), 2), //Step left bottom most
        // new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(3.0f, -1.3f, 1.0f), 2),

        new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(1.0f, -1.5f, 1.0f), 3), //base platform
        new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(0.0f, -0.3f, 1.0f), 0.75), //spawn in middle
        new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(1.1f, -0.12f, 1.0f), 1), //Step 1 right
        new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(-1.1f, -0.12f, 1.0f), 1), //Step 1 left
        new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(-2.2f, -1.3f, 1.0f), 1), //Step left 1
        new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(-1.6f, -1.3f, 1.0f), 1), //Step left 2
        new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(-3.0f, 0.8f, 1.0f), 2), //Step left bottom most
        new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(3.0f, -0.3f, 1.0f), 2),
        jan,
    });
}

void Level1::ChooseCharacter(Character *character) {
    player = character;
    action->AddEntity(player);
    ulttimer = player->threshold;
}

void Level1::Render() {
    CompoundScene::Render();
}
//
//