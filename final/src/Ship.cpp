#include "game/playable/Ship.h"

ShipComponent::ShipComponent(Mass m) 
  : mass(m) {}

Hull::Hull(int h, Mass m) 
  : ShipComponent(m),
    health(h) {}

Battery::Battery(Energy c, Mass m)
  : ShipComponent(m),
    capacity(c),
    current(0.0f, c.GetUnit()) {}

Battery& Battery::operator<<(Reactor& r) {
  return *this;  
}

Engine::Engine(Force t, Mass m) 
  : ShipComponent(m),
    thrust(t) {}

void Engine::Burn() {
  std::cout << "BURN!!!!!" << std::endl;
  // TODO: Complete
} // Burn engine, needs power, produces force

void Engine::operator<<(Battery& b) {
  // TODO: Complete
}

void Engine::operator<<(Reactor& r) {
  // TODO: Complete
}

Thruster::Thruster(Torque t, Mass m) 
  : ShipComponent(m), 
    torque(t) {}

void Thruster::Burn() {
  // TODO: Complete
}

Shield::Shield(Energy c)
  : ShipComponent(Mass(Large(0.0f, 0), Mass::Kilogram)),
    capacity(c),
    current(0.0f, c.GetUnit())
{}

Reactor::Reactor(Power p, Mass m)
  : ShipComponent(m),
    output(p),
    produced(0.0f, Energy::Joule)
{}

ShipConfiguration::ShipConfiguration(Hull h, Engine e, Battery b, Reactor r, Shield s,
  Thruster _rollThruster, Thruster _yawThruster)
  : hull(h),
    engine(e),
    battery(b),
    reactor(r),
    shield(s),
    rollThruster(_rollThruster),
    yawThruster(_yawThruster)
{}

ShipConfiguration::ShipConfiguration(const ShipConfiguration& original) 
  : hull(original.hull),
    engine(original.engine),
    battery(original.battery),
    reactor(original.reactor),
    shield(original.shield),
    rollThruster(original.rollThruster),
    yawThruster(original.yawThruster)
{}

Ship::Ship(Position p, Orientation o, Cylinder s, ShipConfiguration c, Mesh *m, ShaderProgram *sp) 
  : ControllableEntity(),
    PhysicsEntity(p, o, s, s.mass, m, sp),
    config(c)
{
  transformation->Transform(Scale(s.radius.GetLength(Length::Game)));
}