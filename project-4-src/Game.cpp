#include "include/Game.h"
#include <string>

Game::Game (float screenHeight, float screenWidth)
  : top((screenHeight / screenWidth) * 5.0f), bottom(-1 * top), left(-5.0f), right(5.0f)
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

  // Entity Initialization
  ship = Ship(&rendererUntextured);
  foreground.push_back(&ship);

  for (int i = 0; i < NUM_STARS; ++i) {
    stars[i] = Star(&rendererUntexturedBackground, rand(), left * 2, right * 2, top * 2, bottom * 2);
    background.push_back(&stars[i]);
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
      // background[i]->SetRenderFlag(background[i]->CheckBounds(pLeft, pRight, pTop, pBottom));
    }
    // Render the main game
    ship.Render();
    for (int i = 0; i < NUM_STARS; ++i) {
      stars[i].Render();
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
    CameraPan(glm::vec3(0.5f, 0.5f, 0.0f) * FIXED_TIMESTEP);
    if (gameState == 1) {
      // Update the main menu based on user inputs
    }
    if (gameState == 2) {
      // Get current world state

      // Make necessary updates
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

      // Check if keys are still up
      if (!keys[SDL_SCANCODE_LEFT] && !keys[SDL_SCANCODE_RIGHT]) {
	
      }
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
