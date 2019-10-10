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

  // Initialize elements
  lander = Lander(&rendererTextured, glm::vec3(left * 0.9f,top * 0.85f, 1.0f), 0.2f, "textures/lander-sheet.png", 1000, 1.0f, 0.25f, 75.0f);
  land = Terrain(&rendererUntextured, left, right, bottom, bottom * 0.4, (int)time(NULL));

  // Intialize text
  fuelLabel = Text("FUEL", &rendererTextured, left * 0.90f - 0.2f, top * 0.9f, 0.1, 0.15, "textures/font-sheet-bold.png");
  altitudeLabel = Text("ALTITUDE", &rendererTextured, right * 0.90f - 0.7f, top * 0.9f, 0.1, 0.15, "textures/font-sheet-bold.png");
  hLabel = Text("X-SPEED", &rendererTextured, right * 0.90f - 0.75f, top * 0.9f - 0.25, 0.1, 0.15, "textures/font-sheet-bold.png");
  vLabel = Text("Y-SPEED", &rendererTextured, right * 0.90f - 0.75f, top * 0.9f - 0.45, 0.1, 0.15, "textures/font-sheet-bold.png");
  altitudeDisplay = Text("00", &rendererTextured, right * 0.90f, top * 0.9f, 0.1, 0.15, "textures/font-sheet-bold.png");
  horizontalSpeedDisplay = Text("00", &rendererTextured, right * 0.90f + 0.05, top * 0.9f - 0.25, 0.1, 0.15, "textures/font-sheet-bold.png");
  verticalSpeedDisplay = Text("00", &rendererTextured, right * 0.90f + 0.05, top * 0.9f - 0.45, 0.1, 0.15, "textures/font-sheet-bold.png");
  fuelCount = Text("1000", &rendererTextured, left * 0.90f + 0.4f, top * 0.9f, 0.1, 0.15, "textures/font-sheet-bold.png");
  endingText = Text("MISSION FAILED", &rendererTextured, 0.0f, top * 0.7f, 0.15, 0.25, "textures/font-sheet-bold.png");
  menu[0] = Text("> RETRY <", &rendererTextured, 0.0f, top * 0.7f - 0.3, 0.1, 0.15, "textures/font-sheet-bold.png");
  menu[1] = Text("MAIN MENU", &rendererTextured, 0.0f, top * 0.7f - 0.5, 0.1, 0.15, "textures/font-sheet-bold.png");
  menu[2] = Text("QUIT", &rendererTextured, 0.0f, top * 0.7f - 0.7, 0.1, 0.15, "textures/font-sheet-bold.png");
  mainMenu[0] = Text("> MOON <", &rendererTextured, 0.0f, 0.0f, 0.1, 0.15, "textures/font-sheet-bold.png");
  mainMenu[1] = Text("MARS", &rendererTextured, 0.0f, -0.3f, 0.1, 0.15, "textures/font-sheet-bold.png");
  mainMenu[2] = Text("EXIT", &rendererTextured, 0.0f, -0.6f, 0.1, 0.15, "textures/font-sheet-bold.png");
  title = Text("SPACE LANDER", &rendererTextured, 0.0f, 1.0f, 0.15, 0.22, "textures/font-sheet-bold.png");
  description = Text("Can you land your ship safely or will you crash?", &rendererTextured, 0.0f, 0.5f, 0.1, 0.15, "textures/font-sheet-bold.png");

  menuLength = 3;
  selectedMenuIndex = 0;
  
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

void Game::Restart() {
  lastTicks = (float)SDL_GetTicks() / 1000.0f;
  accumulator = 0;
  
  // Reinitialize elements
  lander.Restart(glm::vec3(left * 0.9f, top * 0.85f, 1.0f), 1000);
  if (isMoon) {
    land.Generate(50, -2, 2, 10);
  }
  else {
    land.Generate(50, -2, 2, 25);
  }
  
  // Reintialize text
  fuelCount.SetText(std::to_string(lander.GetFuel()));
  endingText.SetText("MISSION FAILED");
  menu[0].SetText("> RETRY <");
  menu[1].SetText("MAIN MENU");
  menu[2].SetText("QUIT");

  gameState = 2;
}

void Game::Shutdown() {
  SDL_Quit();
}

void Game::Render () {
  glClear(GL_COLOR_BUFFER_BIT);

  if (gameState == 1) {
    title.Render();
    description.Render();
    for (int i = 0; i < menuLength; ++i) {
      mainMenu[i].Render();
    }
  }
  else if (gameState > 1) {
    // Render each element
    lander.Render();
    land.Render();

    // Render text
    fuelCount.Render();
    fuelLabel.Render();
    altitudeLabel.Render();
    altitudeDisplay.Render();
    horizontalSpeedDisplay.Render();
    verticalSpeedDisplay.Render();
    hLabel.Render();
    vLabel.Render();

    if (gameState > 2) {
      endingText.Render();
      for (int i = 0; i < menuLength; ++i) {
	menu[i].Render();
      }
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
      if (menuOptionSelected) {
	if (selectedMenuIndex == 1) {
	  rendererUntextured.SetColor(0.9f, 0.27f, 0.16f, 1.0f);
	  land.Generate(50, -2, 2, 25);
	  isMoon = false;
	  g = GRAVITY;
	  gameState = 2;
	}
	else if (selectedMenuIndex == 0) {
	  g /= 2;
	  rendererUntextured.SetColor(0.7f, 0.7f, 0.7f, 1.0f);
	  land.Generate(50, -2, 2, 10);
	  gameState = 2;
	  isMoon = true;
	}
	else if (selectedMenuIndex == 2) gameState = 0;
	menuOptionSelected = false;
	return;
      }
      else if (optionChanged) {
	for (int i = 0; i < menuLength; ++i) {
	  std::string choiceText = mainMenu[i].GetText();
	  if (choiceText[0] == '>') {
	    mainMenu[i].SetText(choiceText.substr(2, choiceText.size() - 4));
	  }
	  else if (i == selectedMenuIndex) {
	    mainMenu[i].SetText("> " + choiceText + " <");
	  }
	}
	if (selectedMenuIndex == 0) {
	  description.SetText("Lower gravity and more jagged terrain");
	}
	else if (selectedMenuIndex == 1) {
	  description.SetText("Higher gravity and flatter terrain");
	}
	else {
	  description.SetText("Exit the game");
	}
	optionChanged = false;
      }
    }
    // If not in the main menu
    if (gameState == 2) {
      // Get current world state
      int currFuel = lander.GetFuel();
      int altitude = CheckCollision(&lander, &land);
      bool crashed = (altitude < -1);
      glm::vec3 landerSpeed = lander.GetSpeed();
      int horizontalSpeed = landerSpeed[0] * 100;
      int verticalSpeed = landerSpeed[1] * 100;
      // End of getting state
      
      // Update text
      std::string fuelString = std::to_string(currFuel);
      std::string altitudeString = std::to_string(altitude);
      std::string hspeedString = std::to_string(horizontalSpeed);
      std::string vspeedString = std::to_string(verticalSpeed);
      while (fuelString.size() < 4) fuelString = "0" + fuelString;
      while (altitudeString.size() < 3) altitudeString = "0" + altitudeString;
      if (hspeedString[0] == '-' && hspeedString.size() < 4) {
	hspeedString[0] = '0';
	while (hspeedString.size() < 3) hspeedString = "0" + hspeedString;
	hspeedString = "-" + hspeedString;
      }
      else {
	while (hspeedString.size() < 4) hspeedString = "0" + hspeedString;
      }
      if (vspeedString[0] != '-') {
	while (vspeedString.size() < 4) vspeedString = "0" + vspeedString;
      }
      else if (vspeedString.size() < 4) {
        vspeedString[0] = '0';
	while (vspeedString.size() < 3) vspeedString = "0" + vspeedString;
	vspeedString = "-" + vspeedString;
      }
      fuelCount.SetText(fuelString);
      altitudeDisplay.SetText(altitudeString);
      horizontalSpeedDisplay.SetText(hspeedString);
      verticalSpeedDisplay.SetText(vspeedString);
      // End update text

      // Physics calculations
      Gravity(&lander); // Gravity
      if (isThrusting) lander.Thrust(); // Thrust
      // End of physics

      // Collision handling
      if (altitude < 0) { // If the ship is at the surface
	glm::vec3 landerForce = lander.GetForce();
	glm::vec3 reactiveForce = ((landerForce[1] < 0) * -1.0f) * landerForce;
	lander.ApplyForce(reactiveForce); // Reactive force
	lander.Collision(!crashed); // Tell the lander that it has collided with the ground
	if (gameState == 2) {
	  gameState = 3;
	  selectedMenuIndex = 0;
	  menuLength = 3;
	  if (!crashed) endingText.SetText("MISSION SUCCESSFUL");
	}
      }
      else if (altitude < 50) { // Zoom in if the ship is less than 50 ft from the surface
	lander.ZoomTo(2.0f, glm::vec3(lander.GetPos()[0] * -1.0f, lander.GetPos()[1] * -1.0f, 0.0f));
	land.ZoomTo(2.0f, glm::vec3(-1.0f, -1.0f, 1.0f) * lander.GetPos());
      }
      else { // Otherwise zoom out
	glm::vec3 landerPos = lander.GetPos();
	float leftBounds = left * 0.9f;
	float rightBounds = right * 0.9f;
	float topBounds = top * 0.9f;

	glm::vec3 landerFocus = glm::vec3(0.0f);
	glm::vec3 landFocus = glm::vec3(0.0f);
	
	if (landerPos[0] < leftBounds) {
	  landFocus[0] = leftBounds - landerPos[0];
	  landerFocus[0] = leftBounds - landerPos[0];
	}
	else if (landerPos[0] > rightBounds) {
	  landFocus[0] = rightBounds - landerPos[0];
	  landerFocus[0] = rightBounds - landerPos[0];
	}
	if (landerPos[1] > topBounds) {
	  landFocus[1] = topBounds - landerPos[1];
	  landerFocus[1] = topBounds - landerPos[1];
	}
	land.ZoomTo(1.0f, landFocus);
	lander.ZoomTo(1.0f, landerFocus);
      }
    }
    else if (gameState == 3) { // If the game has ended...
      if (menuOptionSelected) {
	if (selectedMenuIndex == 0) Restart();
	else if (selectedMenuIndex == 1) {
	  Restart();
	  gameState = 1;
	}
	else gameState = 0;
	menuOptionSelected = false;
	return;
      }
      else if (optionChanged) {
	for (int i = 0; i < menuLength; ++i) {
	  std::string choiceText = menu[i].GetText();
	  if (choiceText[0] == '>') {
	    menu[i].SetText(choiceText.substr(2, choiceText.size() - 4));
	  }
	  else if (i == selectedMenuIndex) {
	    menu[i].SetText("> " + choiceText + " <");
	  }
	}
	optionChanged = false;
      }
    }
    // Update the lander
    lander.Update(FIXED_TIMESTEP);
    
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
	if (gameState == 2) {
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
	else if (gameState != 2) {
	  switch (e.key.keysym.sym) {
	  case SDLK_UP :
	    optionChanged = true;
	    selectedMenuIndex = ((selectedMenuIndex - 1 >= 0) * selectedMenuIndex) + ((selectedMenuIndex - 1 < 0) * menuLength) - 1;
	    break;
	  case SDLK_DOWN :
	    optionChanged = true;
	    selectedMenuIndex = (selectedMenuIndex + 1) % menuLength;
	    break;
	  case SDLK_RETURN :
	    menuOptionSelected = true;
	    break;
	  default:
	    break;
	  }
	}
      }

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

int Game::CheckCollision (Lander *_lander, Terrain *_terrain) {
  glm::mat4x3 aBounds = _lander->GetBounds();
  glm::mat4x3 bBounds = _terrain->GetBounds(glm::vec3(aBounds[0][0], aBounds[3][0], 0.0f));
  float rotation = _lander->GetRotation();

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
