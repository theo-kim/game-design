#include "include/Game.h"
#include <string>

Game::Game (float screenHeight, float screenWidth)
  : top((screenHeight / screenWidth) * 5.0f), bottom(-1 * top), left(-5.0f), right(5.0f),
    width(screenWidth), height(screenHeight),
    collisionTree(glm::vec3(0.0f, 0.0f, 1.0f), 100, 100)
{
  // Background perspective bounds
  pTop = top;
  pBottom = bottom;
  pLeft = left;
  pRight = right;
  
  // Set the gamestate
  gameState = 2;

  // Set up the window
  SDL_Init(SDL_INIT_VIDEO);
  displayWindow = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)screenWidth, (int)screenHeight, SDL_WINDOW_OPENGL);
  SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
  SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
  glewInit();
#endif

  glViewport(0, 0, (int)screenWidth, (int)screenHeight);

  viewMatrix = glm::mat4(1.0f);
  projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
  backgroundProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
}

void Game::Initialize () {
  // Initialize tick count
  lastTicks = (float)SDL_GetTicks() / 1000.0f;
  accumulator = 0;

  // Randomization seeding
  srand(time(NULL));
  
  // Initialize the renderers
  rendererTextured.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
  rendererUntextured.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
  rendererUntexturedBackground.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

  rendererTextured.SetProjectionMatrix(projectionMatrix);
  rendererTextured.SetViewMatrix(viewMatrix);
  rendererTextured.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

  rendererUntextured.SetProjectionMatrix(projectionMatrix);
  rendererUntextured.SetViewMatrix(viewMatrix);

  rendererUntexturedBackground.SetProjectionMatrix(backgroundProjectionMatrix);
  rendererUntexturedBackground.SetViewMatrix(viewMatrix);

  // Assign Render Program
  glUseProgram(rendererTextured.programID);
  glUseProgram(rendererUntextured.programID);
  glUseProgram(rendererUntexturedBackground.programID);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Background coloring
  glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

  // Renderers
  ShaderProgram *renderers[] = { &rendererTextured, &rendererUntextured }; 
  
  // Load textures
  heroTexture = new TextureSheet("textures/hero.png", 1, 1);
  dualGunTexture = new TextureSheet("textures/dual-gun.png", 1, 1);
  enemyTexture = new TextureSheet("textures/enemy.png", 1, 1);
  singleGunTexture = new TextureSheet("textures/single-gun.png", 1, 1);
  
  // Entity Initialization
  ship = new HeroShip(renderers, heroTexture, dualGunTexture, &collisionTree);
  foreground.push_back(ship);

  Spawn(10, 1);

  for (int i = 0; i < NUM_STARS; ++i) {
    stars[i] = new Star(&rendererUntexturedBackground, rand(), left * 2, right * 2, top * 2, bottom * 2);
    background.push_back(stars[i]);
  }
}

void Game::Run() {
  while (gameState != 0) {
    Input();
    Update();
    Render();
  }
}

void Game::Shutdown() {
  delete ship;
  for (int i = 0; i < NUM_STARS; ++i) {
    delete stars[i];
  }
  for (int i = 0; i < enemies.size(); ++i) {
    delete enemies[i];
  }
  SDL_Quit();
}

void Game::Render () {
  glClear(GL_COLOR_BUFFER_BIT);

  if (gameState == 1) {
    // Render the main men\u
  }
  else {
    // Check for entities that are within the camera fram
    for (int i = 0; i < foreground.size(); ++i) {
      foreground[i]->SetRenderFlag(foreground[i]->CheckBounds(left, right, top, bottom));
    }

    for (int i = 0; i < background.size(); ++i) {
      background[i]->SetRenderFlag(background[i]->CheckBounds(pLeft, pRight, pTop, pBottom));
    }

    // Render the main game
    
    for (int i = 0; i < NUM_STARS; ++i) {
      stars[i]->Render();
    }

    ship->Render();

    for (int i = 0; i < enemies.size(); ++i) {
      enemies[i]->Render();
    }
  }
  
  SDL_GL_SwapWindow(displayWindow);
}

void Game::Update () {
  // Get the ticks
  float ticks = (float)SDL_GetTicks() / 1000.0f;
  float delta = ticks - lastTicks;
  lastTicks = ticks;
  
  delta += accumulator;
  if (delta < FIXED_TIMESTEP) {
    accumulator = delta;
  }
  // END OF TICK CALCULATION
  
  while (delta >= FIXED_TIMESTEP) {
    if (gameState == 1) {
      // Update the main menu based on user inputs
    }
    if (gameState == 2) {
      // Get current world state

      // Make necessary updates
      ship->Update(FIXED_TIMESTEP);
      CameraPan(ship->GetMov() * FIXED_TIMESTEP);

      for (int i = 0; i < enemies.size(); ++i) {
	enemies[i]->Update(FIXED_TIMESTEP);
      }
      
      // Check collisions
      collisionTree.CheckCollision();
    }
    else if (gameState == 3) { // If the game has ended...
      // Render the GAMEOVER view
    }
    delta -= FIXED_TIMESTEP;
  }

  accumulator = delta;
}

void Game::Input () {
  SDL_Event e;
  // Listen for events
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT || e.type == SDL_WINDOWEVENT_CLOSE) {
      // Stop the game
      gameState = 0;
    }
    else {
      const Uint8 *keys = SDL_GetKeyboardState(NULL);
  
      if (e.type == SDL_KEYDOWN) {
        // Get keydown events
      }
      else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && fireState == 0) {
	ship->Fire(0);
	fireState = 1;
      }
      else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
	fireState = 0;
      }

      // Ship movement
      ship->Move(keys[SDL_SCANCODE_W], keys[SDL_SCANCODE_S], keys[SDL_SCANCODE_A], keys[SDL_SCANCODE_D]);

      // Ship gun rotation
      float mouseAngle = 0.0f;
      int x, y;
      Uint32 mouseButtonState = SDL_GetMouseState(&x, &y);

      mouseAngle = -1.0f * glm::atan((float)y - height / 2, (float)x - width / 2);
      
      ship->MoveGun(mouseAngle);
    }
  }
}

// Camera Methods
void Game::CameraZoom(float zoomFactor) {
  projectionMatrix = glm::scale(projectionMatrix, glm::vec3(zoomFactor, zoomFactor, 1.0f));
  rendererTextured.SetProjectionMatrix(projectionMatrix);
  rendererUntextured.SetProjectionMatrix(projectionMatrix);
}

void Game::CameraPan(glm::vec3 pan) {
  float parallaxFactor = 0.05;
  
  left += pan[0];
  right += pan[0];
  top += pan[1];
  bottom += pan[1];

  pLeft += pan[0] * parallaxFactor;
  pRight += pan[0] * parallaxFactor;
  pTop += pan[1] * parallaxFactor;
  pBottom += pan[1] * parallaxFactor;

  projectionMatrix = glm::translate(projectionMatrix, -1.0f * pan);
  backgroundProjectionMatrix = glm::translate(backgroundProjectionMatrix, -1.0f * pan * parallaxFactor);
  
  rendererTextured.SetProjectionMatrix(projectionMatrix);
  rendererUntextured.SetProjectionMatrix(projectionMatrix);
  rendererUntexturedBackground.SetProjectionMatrix(backgroundProjectionMatrix);
}

void Game::CameraRotate(float rotationFactor) {
  // TODO: Implement
}

// Game methods
void Game::Spawn(int n, int difficulty) {
  enemies.reserve(n * 2);
  for (int i = 0; i < n; ++i) {
    int randomX = random() % 10 - 5;
    int randomY = random() % 10 - 5;
    
    enemies.push_back(new EvilShip(&rendererTextured, enemyTexture, glm::vec3(randomX, randomY, 1.0f), &collisionTree));
    foreground.push_back(enemies[i]);

    Bullet b;
    enemies[i]->AddGun(Gun(&rendererTextured, enemies[i], b, singleGunTexture, glm::vec3(-0.48f, -0.58f, 0.0f), glm::vec3(0.25f, 0.25f, 1.0f)));
    enemies[i]->AddGun(Gun(&rendererTextured, enemies[i], b, singleGunTexture, glm::vec3(0.48f, -0.58f, 0.0f), glm::vec3(0.25f, 0.25f, 1.0f)));
    enemies[i]->AddGun(Gun(&rendererTextured, enemies[i], b, singleGunTexture, glm::vec3(0.0f, 0.75f, 0.0f), glm::vec3(0.25f, 0.25f, 1.0f)));
  }
}
