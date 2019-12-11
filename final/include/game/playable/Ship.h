#ifndef SHIP_DEF
#define SHIP_DEF

#include "standard.h"
#include "physics/entity/PhysicsEntity.h"
#include "physics/properties/Time.h"
#include "physics/properties/Energy.h"
#include "ControllableEntity.h"

class Ship;
class Engine;
class Thruster;
class Battery;
class Hull;
class Reactor;
class Shield;

class ShipComponent /*: public PhysicsEntity*/ {
public:
  ShipComponent(Mass mass);
private:
  Mass mass;
};

// This is the hull (physical body) of the ship
class Hull : public ShipComponent {
public:
  Hull(int health, Mass mass);
private:
  int health;
};

// This is the storage of the ship, stores excess power produced by reactor
class Battery : public ShipComponent {
public:
  Battery(Energy capacity, Mass m);
  Battery& operator<<(Reactor& r);
private:
  Energy capacity;
  Energy current;
};

// This is the linear propulsion of the ship
class Engine : public ShipComponent {
public:
  Engine(Force thrust, Mass m);
  void Burn(); // Burn engine, needs power, produces force
  void operator<<(Battery& b);
  void operator<<(Reactor& r);
private:
  const Force thrust;
};

// This is the angular propulsion of the ship
class Thruster : public ShipComponent {
public:
  Thruster(Torque thrust, Mass m);
  void Burn(); // Burn thruster, needs power, produces torque
  void operator<<(Battery& b);
  void operator<<(Reactor& r);
private:
  Torque torque;
};

// This is the shield generator (absorbs energy)
class Shield : public ShipComponent {
public: 
  Shield(Energy capacity);
  void Regenerate(int energy); // uses energy to increase energy
  void Degenerate(int energy); // absorbs energy, reduces health
  void operator<<(Battery& b); // Wire connection from battery to shield
  void operator<<(Reactor& r); // Wire connection from reactor to shield
private:
  Energy capacity; // how much energy it can still hold
  Energy current;
};

// This is the energy generator, 
class Reactor : public ShipComponent {
public:
  Reactor(Power p, Mass m);
  int Generate(Time delta); // Produce energy in a given time

  // Friend declarations
  friend Battery& Battery::operator<<(Reactor& r);
  friend void Engine::operator<<(Reactor& r);
  friend void Thruster::operator<<(Reactor& r);
  friend void Shield::operator<<(Reactor& r);
private:
  Power output; // energy produced in a time
  Energy produced; // energy available from generate
};

class ShipConfiguration {
public:
  ShipConfiguration(Hull h, Engine e, Battery b, Reactor r, Shield s,
    Thruster rollThruster, Thruster yawThruster);
  // Copy constructor
  ShipConfiguration(const ShipConfiguration& original);

  Hull hull;
  Engine engine;
  Battery battery;
  Reactor reactor;
  Shield shield;
  Thruster rollThruster;
  Thruster yawThruster;
  // Thruster pitchThruster;
};

class Ship : public ControllableEntity, virtual public PhysicsEntity {
public:
  Ship(Position p, Orientation o, Cylinder s, ShipConfiguration c, 
    Mesh *m, ShaderProgram *sp);
  ShipConfiguration config;
};

#endif