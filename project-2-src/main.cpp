#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cmath>

#include "include/standard.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "include/Paddle.h"
#include "include/Pong.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Window* displayWindow;
bool gameIsRunning = true;

float lastTicks = 0.0f;

GLuint LoadTexture(const char* filePath) {
  int w, h, n;
  unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    
  if (image == NULL) {
    std::cout << "Unable to load image. Make sure the path is correct\n";
    assert(false);
  }
    
  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
  stbi_image_free(image);
  return textureID;
}

void Initialize(Pong *game) {
  // Initialize tick count
  lastTicks = (float)SDL_GetTicks() / 1000.0f;

  // Set up the window
  SDL_Init(SDL_INIT_VIDEO);
  displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
  SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
  SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
  glewInit();
#endif

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  game->programTextured.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
  game->programUntextured.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

  game->programTextured.SetProjectionMatrix(game->projectionMatrix);
  game->programTextured.SetViewMatrix(game->viewMatrix);
  game->programTextured.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

  game->programUntextured.SetProjectionMatrix(game->projectionMatrix);
  game->programUntextured.SetViewMatrix(game->viewMatrix);
  game->programUntextured.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

  game->InitText(LoadTexture("textures/font.png"));
  
  // Assign Render Program
  glUseProgram(game->programTextured.programID);
  glUseProgram(game->programUntextured.programID);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Background coloring
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ProcessInput(Pong *game) {
  SDL_Event event;
  // Listen for events
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
      gameIsRunning = false;
    }
    else {
      game->Input(event);
    }
  }
}

void Update(Pong *game) { 
  float ticks = (float)SDL_GetTicks() / 1000.0f;
  float deltaTime = ticks - lastTicks;
  lastTicks = ticks;

  game->Update(deltaTime);
}

void Render(Pong *game) {
  glClear(GL_COLOR_BUFFER_BIT);
  
  game->Render();
  
  SDL_GL_SwapWindow(displayWindow);
}

void Shutdown(Pong *game) {
  SDL_Quit();
}

int main (int argc, char* argv[]) {
  Pong game((float)SCREEN_HEIGHT, (float)SCREEN_WIDTH);
  Initialize(&game);

  while (gameIsRunning) {
    ProcessInput(&game);
    Update(&game);
    Render(&game);
  }

  Shutdown(&game);
  return 0;
}
