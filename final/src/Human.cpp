#include "game/controllers/Human.h"

Human::Human(Ship *s) : Controller(s), ship(s) {
  keys[SDLK_a] = false;
  keys[SDLK_d] = false;
  keys[SDLK_w] = false;
  keys[SDLK_s] = false;
  keys[SDLK_SPACE] = false;
  keys[SDL_BUTTON_LEFT] = false;
  keys[SDL_BUTTON_RIGHT] = false;
}

void Human::Input(SDL_Event& e) {
  // Camera tracking
  if (e.type == SDL_MOUSEMOTION && keys[SDL_BUTTON_RIGHT]) {
    ship->config.cockpit.RotateView(Rotation(radians(-e.motion.yrel / 10.0f), radians(-e.motion.xrel / 10.0f), 0.0f));
  }
  else if (e.type == SDL_MOUSEMOTION && !ship->dead) {
    ship->currentAngularVelocity = AngularVelocity(glm::vec3(radians(-e.motion.yrel), radians(-e.motion.xrel), 0.0f), Time::Seconds);
  }
  else if (e.type == SDL_MOUSEBUTTONDOWN) {
    keys[e.button.button] = true;
  }
  else if (e.type == SDL_MOUSEBUTTONUP) {
    keys[e.button.button] = false;
  }
  else if (e.type == SDL_KEYDOWN) {
    keys[e.key.keysym.sym] = true;
    if (e.key.keysym.sym == SDLK_ESCAPE) {
      ship->currentAcceleration = glm::vec3(0.0f);
      ship->currentPosition = Position(glm::vec3(0.0f), Length::Kilometer);
      ship->currentVelocity = glm::vec3(0.0f);
      ship->currentOrientation = Orientation(glm::vec3(0.0f));
      ship->currentAngularVelocity = AngularVelocity(glm::vec3(0.0f), Time::Seconds);
      ship->transformation->translation = Translation();
      ship->transformation->rotation = Rotation();
      for (ShipFuel *f : ship->fuelTrail) {
        delete f;
      }
      ship->fuelTrail.clear();
      ship->dead = false;
      ship->transformation->scale = Scale(ship->shape->radius.GetLength(Length::Game));
    }
  }
  else if (e.type == SDL_KEYUP) {
    keys[e.key.keysym.sym] = false;
  }
  else {
    ship->currentAngularVelocity = AngularVelocity(glm::vec3(0,0, 0.0f), Time::Seconds);
  }
  // else if (e.type == SDL_KEYUP) {
  //   if (e.key.keysym.sym == SDLK_a) {
  //     ship->config.rollThruster.Burn(Thruster::Forwards);
  //   }
  //   if (e.key.keysym.sym == SDLK_d) {
  //     ship->config.rollThruster.Burn(Thruster::Backwards);
  //   }
  //   if (e.key.keysym.sym == SDLK_w) {
  //     ship->config.yawThruster.Burn(Thruster::Backwards);
  //   }
  //   if (e.key.keysym.sym == SDLK_s) {
  //     ship->config.yawThruster.Burn(Thruster::Forwards);
  //   }
  // }
}

void Human::Update(float delta) {
  if (keys[SDL_BUTTON_LEFT]) {
    ship->config.engine.Burn();
  }
  // if (keys[SDLK_d]) {
  //   ship->config.rollThruster.Burn(Thruster::Backwards);
  // }
  // if (keys[SDLK_w]) {
  //   ship->config.yawThruster.Burn(Thruster::Backwards);
  // }
  // if (keys[SDLK_s]) {
  //   ship->config.yawThruster.Burn(Thruster::Forwards);
  // }
  // if (keys[SDLK_SPACE]) {
  //   ship->config.engine.Burn();
  // }
}