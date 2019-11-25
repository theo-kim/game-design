#include <iostream>
#include <stdlib.h>

#include "../include/Game.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

Game game((float)SCREEN_HEIGHT, (float)SCREEN_WIDTH);

int main (int argc, char* argv[]) {
  game.Initialize();
  game.Run();
  game.Shutdown();
  return 0;
}
