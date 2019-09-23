#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cmath>

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ShaderProgram.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "include/Star.h"
#include "include/Ship.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define NUM_STARS 30

SDL_Window* displayWindow;
bool gameIsRunning = true;

float lastTicks = 0.0f;

glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

ShaderProgram program;

GLuint playerTextureID;

Star stars[NUM_STARS];
Ship enterprise;
Ship klingon;

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

void Initialize() {
  // Set up random number generator
  srand(time(NULL));
  lastTicks = (float)SDL_GetTicks() / 1000.0f;

  SDL_Init(SDL_INIT_VIDEO);
  displayWindow = SDL_CreateWindow("Errand of Mercy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
  SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
  SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
  glewInit();
#endif

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

  enterprise.textureID = LoadTexture("./textures/enterprise.png");
  klingon.textureID = LoadTexture("./textures/birdofprey.png");

  viewMatrix = glm::mat4(1.0f);
  projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

  program.SetProjectionMatrix(projectionMatrix);
  program.SetViewMatrix(viewMatrix);
  program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

  glUseProgram(program.programID);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Initialize the stars
  for (int i = 0; i < NUM_STARS; ++i) {
    stars[i].Create(rand());
  }

  enterprise.Create(0.0f, 0.0f, 0.8f, 0.8f, -90.0f);
  klingon.Create(2.0f, 0.0f, 0.7f, 0.7f, 0.0f);
  // Background coloring
  glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
}

// Not needed for this project :-(
void ProcessInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
      gameIsRunning = false;
    }
  }
}

void Update() { 
  glm::vec3 globalMovement = glm::vec3(1.5f, 0.0f, 0.0f);
  
  float ticks = (float)SDL_GetTicks() / 1000.0f;
  float deltaTime = ticks - lastTicks;
  lastTicks = ticks;

  for (int i = 0; i < NUM_STARS; ++i) {
    stars[i].Reposition(&globalMovement, deltaTime);
  }

  klingon.rotation += 20 * deltaTime;
  klingon.pos[0] = 0.0f + std::cos(glm::radians(40 * deltaTime)) * 2.0f;
  klingon.pos[1] = 0.0f + std::sin(glm::radians(40 * deltaTime)) * 2.0f;
}

void Render() {
  glClear(GL_COLOR_BUFFER_BIT);
  
  for (int i = 0; i < NUM_STARS; ++i) {
    stars[i].Display(&program);
  }  
  
  enterprise.Display(&program);
  klingon.Display(&program);
  
  SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
  SDL_Quit();
}

int main (int argc, char* argv[]) {
  Initialize();

  while (gameIsRunning) {
    ProcessInput();
    Update();
    Render();
  }

  Shutdown();
  return 0;
}
