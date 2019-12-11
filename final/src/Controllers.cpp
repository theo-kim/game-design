#include "game/controllers/Collision.h"

CollisionController::CollisionController(ControllableEntity *c, GameScene *s)
  : Controller(c),
    object(c),
    scene(s)
{}

void CollisionController::ClearState() {
  checked.clear();
}

void CollisionController::Input(SDL_Event& e) {
  
}

void CollisionController::Update(float delta) {
  ClearState();
  // Displacement d = (object->currentPosition - scene->asteroids[2]->currentPosition);
  // std::cout << d.scalar(Length::Meter) << std::endl;
  // if (d.scalar(Length::Meter) < pow(2 * (object->shape->radius.GetLength(Length::Meter) + scene->asteroids[2]->shape->radius.GetLength(Length::Meter)), 2)) {
  //   // Collision has occured!
  //   currentState = Controller::State(1);
  // }
  for (Asteroid *s : scene->asteroids) {
    Displacement d = (object->currentPosition - s->currentPosition);
    // std::cout << (d.scalar(Length::Meter) < (float)1.0e9) << std::endl;
    
    if (d.scalar(Length::Meter) < pow(2 * (object->shape->radius.GetLength(Length::Meter) + s->shape->radius.GetLength(Length::Meter)), 2)) {
      // Collision has occured!
      Ship * s = dynamic_cast<Ship *>(object);
      s->DidCollide();
    }
  }
  Displacement d = (object->currentPosition - scene->station->currentPosition);
  
  if (d.scalar(Length::Meter) < pow(2 * (object->shape->radius.GetLength(Length::Meter) + scene->station->shape->radius.GetLength(Length::Meter)), 2)) {
    currentState = Controller::State(1);
  }
}