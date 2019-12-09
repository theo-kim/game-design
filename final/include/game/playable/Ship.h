#ifndef SHIP_DEF
#define SHIP_DEF

#include "standard.h"
#include "physics/entity/PhysicsEntity.h"
#include "physics/properties/Time.h"
#include "ControllableEntity.h"

class Ship;
class Engine;
class Thruster;
class Battery;
class Hull;
class Reactor;
class Shield;

class ShipComponent : public PhysicsEntity {
public:
  ShipComponent(Mass mass, Ship* ship);
private:
  Ship *attachedTo;
};

// This is the hull (physical body) of the ship
class Hull : public ShipComponent {
public:
  Hull(int health, Mass mass, Ship* ship);
private:
  int health;
};

// This is the storage of the ship, stores excess power produced by reactor
class Battery : public ShipComponent {
public:
  Battery();
  Battery& operator<<(Reactor& r);
private:
  int capacity;
};

// This is the linear propulsion of the ship
class Engine : public ShipComponent {
public:
  Engine();
  void Burn(); // Burn engine, needs power, produces force
  void operator<<(Battery& b);
  void operator<<(Reactor& r);
};

// This is the angular propulsion of the ship
class Thruster : public ShipComponent {
public:
  Thruster();
  void Burn(); // Burn thruster, needs power, produces torque
  void operator<<(Battery& b);
  void operator<<(Reactor& r);
};

// This is the shield generator (absorbs energy)
class Shield : public ShipComponent {
public: 
  Shield();
  void Regenerate(int energy); // uses energy to increase energy
  void Degenerate(int energy); // absorbs energy, reduces health
  void operator<<(Battery& b); // Wire connection from battery to shield
  void operator<<(Reactor& r); // Wire connection from reactor to shield
private:
  int health; // how much energy it can still hold
};

// This is the energy generator, 
class Reactor : public ShipComponent {
public:
  Reactor();
  int Generate(Time delta); // Produce energy in a given time

  // Friend declarations
  friend Battery& Battery::operator<<(Reactor& r);
  friend void Engine::operator<<(Reactor& r);
  friend void Thruster::operator<<(Reactor& r);
  friend void Shield::operator<<(Reactor& r);
private:
  int power; // energy produced in a time
  int produced; // energy available from generate
};

class Ship : public ControllableEntity {
public:
  Ship(Position p, Orientation o);
};

#endif