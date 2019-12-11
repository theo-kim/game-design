#include "game/controllers/Human.h"

Human::Human(Ship *s) : Controller(s), ship(s) {}

void Human::Input() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_KEYDOWN) {
      if (e.key.keysym.sym == SDLK_a) {
        
      }
      if (e.key.keysym.sym == SDLK_d) {
        
      }
      if (e.key.keysym.sym == SDLK_w) {

      }
      if (e.key.keysym.sym == SDLK_s) {

      }
      if (e.key.keysym.sym == SDLK_SPACE) {
        ship->config.engine.Burn();
      }
    }
  }
}

void Human::Update(float delta) {
  // Does not need anything because the human sees the screen
}