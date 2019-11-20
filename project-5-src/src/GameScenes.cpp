#include "../include/Scenes.h"
#include "../include/framework/ui/standard.h"
#include "../include/framework/animation/Blink.h"
#include "../include/Character.h"
#include "../include/CharacterTypes.h"
#include "../include/Platform.h"

#define GRAVITY 9.8f

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
      Scene(size, glm::vec3(0.3f)) { }

Scene *MainMenuScene::Update(float delta) {
    CompoundScene::Update(delta);

    int index = menu->GetSelectedIndex();
    if (index != -1) {
        Level1 *next = new Level1(GetDimensions(), glm::vec3(50.0f, 50.0f, 1.0f));
        Unload();
        next->Load();
        if (index == 0) {
            next->ChooseCharacter(new Michael(*(next->GetLayer(0)), *next, *next, glm::vec3(0.0f), 0.25));
        }
        else if (index == 1) {
            next->ChooseCharacter(new Stanley(*(next->GetLayer(0)), *next, *next, glm::vec3(0.0f), 0.25));
        }
        else if (index == 2) {
            next->ChooseCharacter(new Dwight(*(next->GetLayer(0)), *(next->GetLayer(0)), *next, *next, glm::vec3(0.0f), 0.25));
        }
        else {
            next->ChooseCharacter(new Jim(*(next->GetLayer(0)), *next, *next, glm::vec3(0.0f), 0.25));
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
                new Text("Michael", *menu, glm::vec3(-1.5f, -0.7f, 0.0f), 0.15),
                new Text("Scott", *menu, glm::vec3(-1.5f, -0.85f, 0.0f), 0.15)
            })
        ),
        new AnimatedButton(
            new Slide(NULL, 0.2f, glm::vec3(0.0f, 0.2f, 0.0f), false),
            NULL,
            new Container({
                new Stanley(*menu, glm::vec3(-0.5f, 0.0f, 0.0f), 1.0f),
                new Text("Stanley", *menu, glm::vec3(-0.5f, -0.7f, 0.0f), 0.15),
                new Text("Hudson", *menu, glm::vec3(-0.5f, -0.85f, 0.0f), 0.15)
            })
        ),
        new AnimatedButton(
            new Slide(NULL, 0.2f, glm::vec3(0.0f, 0.2f, 0.0f), false),
            NULL,
            new Container({
                new Dwight(*menu, glm::vec3(0.5f, 0.0f, 0.0f), 1.0f),
                new Text("Dwight", *menu, glm::vec3(0.5f, -0.7f, 0.0f), 0.15),
                new Text("Schrute", *menu, glm::vec3(0.5f, -0.85f, 0.0f), 0.15)
            })
        ),
        new AnimatedButton(
            new Slide(NULL, 0.2f, glm::vec3(0.0f, 0.2f, 0.0f), false),
            NULL,
            new Container({
                new Jim(*menu, glm::vec3(1.5f, 0.0f, 0.0f), 1.0f),
                new Text("Jim", *menu, glm::vec3(1.5f, -0.7f, 0.0f), 0.15),
                new Text("Halpert", *menu, glm::vec3(1.5f, -0.85f, 0.0f), 0.15)
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
      ActionScene(maxSize[1], maxSize[0], size[0], size[1], size[2], GRAVITY),
      Scene(size, glm::vec3(0.3f)),
      currentLevel(0)
{
    ulttimer = 0.0f;
}

Scene *Level1::Update(float delta) {
    ActionScene::Update(delta);
    CompoundScene::Update(delta);
    player->Update(delta);
    action->GetCamera()->Update();
    action->UpdateView();
    if (numlives > 0 && (player->currentHealth <= 0 || player->GetPos()[1] < -7.0f)) {
        if (--numlives == 0) {
            player->SetPos(glm::vec3(0.0f));
            player->Walk(0);
            player->SetForce(glm::vec3(0.0f));
            player->mov = glm::vec3(0.0f);
            player->currentHealth = 1;
            info->AddEntity({
                new Text("Game Over!", *info, glm::vec3(0.0f, 1.0f, 1.0f), 0.2f),
                new Blink(new Text("Press Enter to return to menu", *info, glm::vec3(0.0f, 0.75f, 1.0f), 0.15), 1.0f),
            });
        }
        else {
            player->currentHealth = 1;
            player->SetPos(glm::vec3(0.0f));
            player->SetForce(glm::vec3(0.0f));
            player->mov = glm::vec3(0.0f);
        }
    }
    else if (jan->currentHealth <= 0 && robert->currentHealth <= 0) {
        if (++currentLevel > 2) {
            info->AddEntity({
                new Text("You win!", *info, glm::vec3(0.0f), 0.2),
            });
        }
        else {
            GetCollisionEngine()->Clear();
            action->UnloadEntities();
            if (currentLevel == 1) {
                jan = new Jan(*action, GetPhysicsEngine(), GetCollisionEngine(), glm::vec3(1.0f, -1.0f, 0.0f), 0.25f);
                robert = new Robert(*action, *this, *this, glm::vec3(-3.0f, 2.0f, 0.0f), 0.25f);
                action->AddEntity({
                    new Platform(*action, *this, glm::vec3(0.4f), glm::vec3(0.9f, -1.5f, 1.0f), 3), //base platform
                    new Platform(*action, *this, glm::vec3(0.4f), glm::vec3(0.0f, -0.3f, 1.0f), 0.75), //spawn in middle
                    new Platform(*action, *this, glm::vec3(0.4f), glm::vec3(1.1f, -0.12f, 1.0f), 1), //Step 1 right
                    new Platform(*action, *this, glm::vec3(0.4f), glm::vec3(-1.1f, -0.12f, 1.0f), 1), //Step 1 left
                    new Platform(*action, *this, glm::vec3(0.4f), glm::vec3(-2.0f, -1.3f, 1.0f), 1), //Step left 1
                    new Platform(*action, *this, glm::vec3(0.4f), glm::vec3(-1.6f, -1.3f, 1.0f), 1), //Step left 2
                    new Platform(*action, *this, glm::vec3(0.4f), glm::vec3(-3.0f, 0.8f, 1.0f), 2), //Step left bottom most
                    new Platform(*action, *this, glm::vec3(0.4f), glm::vec3(3.0f, -0.3f, 1.0f), 2),
                    new Platform(*action, *this, glm::vec3(0.4f), glm::vec3(2.75f, 0.75f, 1.0f), 1.5),
                    new Platform(*action, *this, glm::vec3(0.4f), glm::vec3(4.25f, 0.3f, 1.0f), 1.0),
                    new Platform(*action, *this, glm::vec3(0.4f), glm::vec3(0.25f, 1.25f, 1.0f), 3.0),
                    new Platform(*action, *this, glm::vec3(0.4f), glm::vec3(-1.75f, 1.5f, 1.0f), 0.5),
                    jan,
                    robert
                });
            }
            else if (currentLevel == 2) {
                jan = new Jan(*action, *this, *this, glm::vec3(0.0f, -1.0f, 0.0f), 0.25f);
                robert = new Robert(*action, *this, *this,  glm::vec3(-3.0f, -0.25f, 0.0f), 0.25f);
                action->AddEntity({
                    //Level 2
                    new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(1.0f, -1.5f, 1.0f), 3), //base platform
                    new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(0.0f, -0.3f, 1.0f), 0.75), //spawn in middle
                    new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(1.1f, 0.12f, 1.0f), 1), //Step 1 right
                    new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(-1.1f, 0.12f, 1.0f), 1), //Step 1 left
                    new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(-2.2f, -0.7f, 1.0f), 1), //Step left 1
                    new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(-1.65f, -1.3f, 1.0f), 1), //Step left 2
                    new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(-3.0f, -1.7f, 1.0f), 2), //Step left bottom most
                    new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(3.0f, -1.0f, 1.0f), 2.5),
                    new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(4.25f, -0.45f, 1.0f), 1),
                    new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(3.75f, 0.15f, 1.0f), 0.75f),
                    new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(4.15f, 0.76f, 1.0f), 0.5f),
                    new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(1.75f, 1.0f, 1.0f), 3.5f),
                    jan,
                    robert
                });
            }
            // action->AddEntity(jan);
            player->SetPos(glm::vec3(0.0f));
            player->SetForce(glm::vec3(0.0f));
            player->mov = glm::vec3(0.0f);
        }
    }
    else {
        ulttimer += delta;
        if (ulttimer >= player->threshold && !player->ulting) ulttimer = player->threshold;
        else if (ulttimer >= player->threshold && player->ulting) ulttimer = 0;
        timer->SetText("Next Ability in " + std::to_string((int)(player->threshold - ulttimer)));
        lives->SetText("Lives Remaining: " + std::to_string(numlives));
        levelLabel->SetText("Current Level: " + std::to_string(currentLevel + 1));
    }

    if (GetNumTransitions() > 0) {
        Scene *next = GetTransition(0);
        Unload();
        next->Load();
        return next;
    }
    return NULL;
}

void Level1::Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, float mouseX, float mouseY) {
    if (numlives <= 0 || currentLevel + 1 > 3) {
        if (keys[SDL_SCANCODE_RETURN]) {
            AddTransition(new MainMenuScene(GetDimensions()));
        }
        return;
    }
    player->Walk(keys[SDL_SCANCODE_D] - keys[SDL_SCANCODE_A]);
    player->Jump(keys[SDL_SCANCODE_SPACE]);
    player->Ult(ulttimer >= player->threshold && keys[SDL_SCANCODE_Q]);
    // CompoundScene::Input(event, keys, mouse, mouseX, mouseY);
}

void Level1::Load() {
    numlives = 3;
    info = new SimpleScene(GetDimensions(), glm::vec4(0.0f));
    action = new SimpleScene(GetDimensions(), glm::vec4(0.0f));
    AddLayer(glm::vec3(0.0f), action);
    AddLayer(glm::vec3(0.0f), info);
    CompoundScene::Load();

    timer = new Text("Next Ability in X", *info, glm::vec3(-(GetDimensions()[0] / 2) + 0.1f, (GetDimensions()[1] / 2) - 0.3f, 0.0f), 0.15, Text::LEFT);
    lives = new Text("Lives Remaining: X", *info, glm::vec3(-(GetDimensions()[0] / 2) + 0.1f, (GetDimensions()[1] / 2) - 0.5f, 0.0f), 0.15, Text::LEFT);
    levelLabel = new Text("Level 1", *info, glm::vec3(-(GetDimensions()[0] / 2) + 0.1f, (GetDimensions()[1] / 2) - 0.1f, 0.0f), 0.15, Text::LEFT);
    info->AddEntity({
        levelLabel,
        timer,
        lives,
    });
    
    jan = new Jan(*action, *this, *this, glm::vec3(0.0f, -1.0f, 0.0f), 0.25f);
    robert = new Robert(*action, *this, *this,  glm::vec3(-3.0f, -1.0f, 0.0f), 0.25f);
    action->AddEntity({
        //Level 2
        new Platform(*action, *this, glm::vec3(0.1f), glm::vec3(0.0f, -1.5f, 1.0f), 8.0f), //base platform
        jan,
        robert
    });
}

void Level1::ChooseCharacter(Character *character) {
    player = character;
    ulttimer = player->threshold;
    action->GetCamera()->Follow(player);
    action->GetCamera()->Zoom(2.0f);
}

void Level1::Render() {
    player->Render();
    CompoundScene::Render();
}
//
//