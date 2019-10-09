#include "include/Game.h"
#include <string>

Game::Game (float screenHeight, float screenWidth) : top((screenHeight / screenWidth) * 5.0f),
						     bottom(-1 * top),
						     left(-5.0f),
						     right(5.0f) {
  // Set the gamestate
  gameState = 1;

  // Intialize physics
  g = GRAVITY;
  
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
}

void Game::Initialize () {
  // Initialize tick count
  lastTicks = (float)SDL_GetTicks() / 1000.0f;
  accumulator = 0;
  
  // Initialize the renderers
  rendererTextured.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
  rendererUntextured.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

  rendererTextured.SetProjectionMatrix(projectionMatrix);
  rendererTextured.SetViewMatrix(viewMatrix);
  rendererTextured.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

  rendererUntextured.SetProjectionMatrix(projectionMatrix);
  rendererUntextured.SetViewMatrix(viewMatrix);
  rendererUntextured.SetColor(0.9f, 0.27f, 0.16f, 1.0f);

  // Initialize elements
  lander = Lander(&rendererTextured, glm::vec3(left * 0.9f,top * 0.85f, 1.0f), 0.2f, "textures/lander-sheet.png", 1000, 1.0f, 0.75f, 75.0f);
  land = Terrain(&rendererUntextured, left, right, bottom, bottom * 0.4, (int)time(NULL));
  land.Generate(50, -2, 2, 20);

  // Intialize text
  fuelLabel = Text("FUEL", &rendererTextured, left * 0.90f - 0.2f, top * 0.9f, 0.1, 0.15, "textures/font-sheet-bold.png");
  fuelCount = Text("1000", &rendererTextured, left * 0.90f + 0.4f, top * 0.9f, 0.1, 0.15, "textures/font-sheet-bold.png");
  endingText = Text("GAME OVER", &rendererTextured, 0.0f, top * 0.7f, 0.15, 0.25, "textures/font-sheet-bold.png");
  
  // Assign Render Program
  glUseProgram(rendererTextured.programID);
  glUseProgram(rendererUntextured.programID);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Background coloring
  glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
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

  // Render each element
  lander.Render();
  land.Render();

  // Render text
  fuelCount.Render();
  fuelLabel.Render();

  if (gameState > 1) {
    endingText.Render();
  }
  
  SDL_GL_SwapWindow(displayWindow);
}

void Game::Update () {
  // Get the frame rate
  float ticks = (float)SDL_GetTicks() / 1000.0f;
  float delta = ticks - lastTicks;
  lastTicks = ticks;

  delta += accumulator;
  if (delta < FIXED_TIMESTEP) {
    accumulator = delta;
  }

  while (delta >= FIXED_TIMESTEP) {
    int currFuel = lander.GetFuel();
    std::string fuelString = std::to_string(currFuel);
    while (fuelString.size() < 4) fuelString = "0" + fuelString;
    fuelCount.SetText(fuelString);
    Gravity(&lander);
    if (isThrusting and currFuel > 0) {
      lander.Thrust();
    }
    int altitude = CheckCollision(&lander, &land);
    bool crashed = (altitude < -1);
    
    if (altitude < 0) {
      glm::vec3 landerForce = lander.GetForce();
      glm::vec3 reactiveForce = glm::vec3(0.0f);
      if (landerForce[1] < 0) {
	reactiveForce = -1.0f * landerForce;
      }
      lander.ApplyForce(reactiveForce); // Reactive force
      lander.Collision(!crashed);
      if (crashed && gameState == 1) {
	gameState = 2;
      }
      else if (!crashed && gameState == 1) {
	gameState = 2;
	endingText.SetText("SUCCESS");
      }
    }
    if (altitude < 50) {
      lander.ZoomTo(2.0f);
      land.ZoomTo(2.0f, glm::vec3(-1.0f, -1.0f, 1.0f) * lander.GetPos());
    }
    else {
      lander.ZoomTo(1.0f);
      land.ZoomTo(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    }
    lander.Update(FIXED_TIMESTEP);
    delta -= FIXED_TIMESTEP;
  }

  accumulator = delta;
}

int Game::CheckCollision (Lander *lander, Terrain *terrain) {
  glm::mat4x3 aBounds = lander->GetBounds();
  glm::mat4x3 bBounds = terrain->GetBounds(glm::vec3(aBounds[0][0], aBounds[3][0], 0.0f));
  float rotation = lander->GetRotation();

  // Transform terrain to line general form
  float A = bBounds[0][1] - bBounds[2][1];
  float B = bBounds[2][0] - bBounds[0][0];
  float C = A * bBounds[0][0] + B * bBounds[0][1];
  float m = A / (-B);
    
  int flags = 0;
  int minYCheck = 10000;
  
  // For each point of the lander bounds
  for (int i = 0; i < 4; ++i) {
    // check is the point is within the segment
    if (aBounds[i][0] > bBounds[0][0] && aBounds[i][0] < bBounds[3][0]) {
      // check if the point is under the line
      float yCheck = (C - A * aBounds[i][0]) / B;
      // get the lander's altitude
      int dist = (int)(fabsf(yCheck - aBounds[i][1]) * 100);
      if (dist < minYCheck) {
	minYCheck = dist;
      }
      if (yCheck >= aBounds[i][1]) {
	++flags;
      }
    }
  }

  if (flags == 0) {
    return minYCheck;
  }
  if (flags == 1) {
    if (m != 0 || rotation > 0.1 || rotation < -0.1) return -2;
    return -1;
  }
  if (rotation != 0) return -2;
  return -1;
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
	switch (e.key.keysym.sym) {
	case SDLK_RIGHT :
	  // Rotate the ship counter clockwise
	  lander.RotateCounterclockwise();
	  break;
	case SDLK_LEFT :
	  // Rotate the ship clockwise
	  lander.RotateClockwise();
	  break;
	case SDLK_SPACE :
	  // Apply force to the ship
	  isThrusting = true;
	  break;
	default:
	  break;
	}
      }
      // else if (e.type == SDL_KEYUP) {
      // 	switch (e.key.keysym.sym) {
      // 	case SDLK_LEFT :
      // 	case SDLK_RIGHT :
      // 	  // Stop rotating ship
      // 	  break;
      // 	case SDLK_SPACE :
      // 	  // Stop thrusters
      // 	  break;
      // 	default:
      // 	  break;
      // 	}
      // }

      if (!keys[SDL_SCANCODE_LEFT] && !keys[SDL_SCANCODE_RIGHT]) {
	lander.StopRotation();
      }
      if (!keys[SDL_SCANCODE_SPACE]) {
	isThrusting = false;
      }
    }
  }
}

// Physics
void Game::Gravity (Lander *object) {
  object->ApplyForce(glm::vec3(0.0f, -1.0f * g * object->GetMass(), 0.0f));
}
