#include "Game.h"
#include "ui/entity/Entity.h"
#include "ui/mesh/Mesh3D.h"
#include "game/environment/Planet.h"
#include "NetworkConnection.h"
#include "game/scenes/GameScene.h"
#include "ui/mesh/Mesh2D.h"

typedef Transformation::Translation Translation;
typedef Transformation::Rotation Rotation;
typedef Transformation::Scale Scale;

Game::Game (float screenHeight, float screenWidth)
  : top((screenHeight / screenWidth) * 5.0f), bottom(-1 * top), left(-5.0f), right(5.0f),
    width(screenWidth), height(screenHeight)
{
  // Set the gamestate
  gameState = 1;

  // Set up the window
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute (SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3); //OpenGL 3+
  SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3); //OpenGL 3.3
  SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //OpenGL core profile

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

  displayWindow = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)screenWidth, (int)screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

  SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
  // SDL_GL_MakeCurrent(displayWindow, context);
  
  SDL_GL_SetSwapInterval( 1 );

  glewExperimental = GL_TRUE; 
  GLenum glewError = glewInit();
  if( glewError != GLEW_OK ) {
    std::cout << "Error Initializing GLEW" << std::endl;
    exit(1);
  }
                   
#ifdef _WINDOWS
  glewInit();
#endif

  // glViewport(0, 0, (int)screenWidth, (int)screenHeight);
  // std::cout << glGetString(GL_VERSION) << std::endl;
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
  // Enable Depth Buffering
  glEnable(GL_DEPTH_TEST);
  // glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  // glDepthRange(0.0f, 1.0f);

  Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
  IMG_Init(IMG_INIT_PNG);
  TTF_Init();

  // Mesh2D::Square = new Mesh2D(
  //   { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 },
  //   {-1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1}
  // );

  scene = new GameScene(displayWindow, width, height);
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
  TTF_Quit();
  SDL_Quit();
}

void Game::Render () {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  scene->Render();
  // for (Entity *e : entities) {
  //   e->Render();
  // }
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
    scene->Input(e);
    // else {
    //   const Uint8 *keys = SDL_GetKeyboardState(NULL);
    //   int x, y;
    //     const Uint32 mouseButtonState = SDL_GetMouseState(&x, &y);
    //   float mx, my;
    //   mx = ((float)x - width / 2) / (width / (right - left));
    //   my = ((float)y - height / 2) / (height / (top - bottom));
    //   if (e.type == SDL_KEYDOWN) {
    //     if (e.key.keysym.sym == SDLK_a) {
    //       camera->Rotate(Rotation(0, 0, radians(1.0f)));
    //     }
    //     if (e.key.keysym.sym == SDLK_d) {
    //       camera->Rotate(Rotation(0, 0, radians(-1.0f)));
    //     }
    //     if (e.key.keysym.sym == SDLK_w) {
    //       camera->Rotate(Rotation(radians(0.5f), 0, 0));
    //     }
    //     if (e.key.keysym.sym == SDLK_s) {
    //       camera->Rotate(Rotation(radians(-0.5f), 0, 0));
    //     }
    //     if (e.key.keysym.sym == SDLK_SPACE) {
    //       camera->Translate(Translation(0, 0, -0.05f));
    //     }
    //   }
    // }
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
    scene->Update(FIXED_TIMESTEP);
    delta -= FIXED_TIMESTEP;
  }

  accumulator = delta;
}