#include "../include/Game.h"

Game::Game (float screenHeight, float screenWidth)
  : top((screenHeight / screenWidth) * 5.0f), bottom(-1 * top), left(-5.0f), right(5.0f),
    width(screenWidth), height(screenHeight)
{

  // Set the gamestate
  gameState = 1;

  // Set up the window
  SDL_Init(SDL_INIT_VIDEO);
  displayWindow = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)screenWidth, (int)screenHeight, SDL_WINDOW_OPENGL);
  SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
  SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
  glewInit();
#endif

  glViewport(0, 0, (int)screenWidth, (int)screenHeight);
}

void Game::Initialize () {
  // Initialize tick count
  lastTicks = (float)SDL_GetTicks() / 1000.0f;
  accumulator = 0;

  // Randomization seeding
  srand(time(NULL));
  
  // Enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
  Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
}

void Game::Run() {
  while (gameState != 0) {
    Input();
    Update();
    Render();
  }
}

// This function shuts down the game and cleans up services
void Game::Shutdown() {
  Mix_CloseAudio();
  SDL_Quit();
}

void Game::Render () {
  glClear(GL_COLOR_BUFFER_BIT);
  
  SDL_GL_SwapWindow(displayWindow);
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
      int x, y;
        const Uint32 mouseButtonState = SDL_GetMouseState(&x, &y);
      float mx, my;
      mx = ((float)x - width / 2) / (width / (right - left));
      my = ((float)y - height / 2) / (height / (top - bottom));
    }
  }
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
    if (!isPaused) {
    }
    delta -= FIXED_TIMESTEP;
  }

  accumulator = delta;
}