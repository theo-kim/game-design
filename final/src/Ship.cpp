#include "game/playable/Ship.h"

ShipComponent::ShipComponent(Mass m) 
  : mass(m),
    parent(nullptr) {}

void ShipComponent::Attach(Ship *ship) {
  parent = ship;
}

void ShipComponent::Update(float delta) {

}

Hull::Hull(int h, Mass m) 
  : ShipComponent(m),
    health(h) {}

Cockpit::Cockpit() 
  : ShipComponent(Mass()) {}

Cockpit::Cockpit(Transformation t, glm::mat3 c) 
  : ShipComponent(Mass()),
    offset(t),
    constraints(c) {}

Transformation *Cockpit::Sight() {
  return &offset;
}

void Cockpit::Update(float delta) {
  // if (parent != nullptr) {
  //   positioning = *parent->transformation + offset;
  // }
}

void Cockpit::RotateView(Transformation::Rotation rotation) {
  offset.Transform(rotation);
  if (constraints[0][2] == 5.0f) {
      if (offset.rotation.x < constraints[0][0]) {
        offset.rotation.x = constraints[0][0];
      }
      else if (offset.rotation.x > constraints[0][1]) {
        offset.rotation.x = constraints[0][1];
      }
    }
    if (constraints[1][2] == 5.0f) {
      if (offset.rotation.y < constraints[1][0]) {
        offset.rotation.y = constraints[1][0];
      }
      else if (offset.rotation.y > constraints[1][1]) {
        offset.rotation.y = constraints[1][1];
      }
    }
    if (constraints[2][2] == 5.0f) {
      if (offset.rotation.z < constraints[2][0]) {
        offset.rotation.z = constraints[2][0];
      }
      else if (offset.rotation.z > constraints[2][1]) {
        offset.rotation.z = constraints[2][1];
      }
    }
}

Battery::Battery(Energy c, Mass m)
  : ShipComponent(m),
    capacity(c),
    current(0.0f, c.GetUnit()) {}

Battery& Battery::operator<<(Reactor& r) {
  return *this;  
}

Engine::Engine(Force t, Mass m) 
  : ShipComponent(m),
    thrust(t) 
{
  effect = Mix_LoadWAV( "sounds/engine.wav" );
}

void Engine::Burn() {
  // TODO: Complete
  Mix_PlayChannel( 1, effect, 0);
  parent->ApplyForce(thrust * parent->currentOrientation);
  parent->ExpelFuel();
  
} // Burn engine, needs power, produces force

void Engine::StopBurn() {
  
}

void Engine::operator<<(Battery& b) {
  // TODO: Complete
}

void Engine::operator<<(Reactor& r) {
  // TODO: Complete
}

Thruster::Thruster(Torque t, Mass m) 
  : ShipComponent(m), 
    torque(t) 
{
  effect = Mix_LoadWAV("sounds/thruster.wav");
}

void Thruster::Burn(Thruster::Direction d) {
  Mix_PlayChannel(2, effect, 0);
  if (d == Forwards) {
    parent->transformation->Transform(Rotation(torque.value[0], torque.value[1], torque.value[2]));
    parent->currentOrientation = parent->currentOrientation + AngularDisplacement(glm::vec3(torque.value[0], torque.value[1], torque.value[2]));
    //parent->ApplyTorque(torque);
  }
  else {
    parent->transformation->Transform(Rotation(-torque.value[0], -torque.value[1], -torque.value[2]));
    parent->currentOrientation = parent->currentOrientation + AngularDisplacement(glm::vec3(-torque.value[0], -torque.value[1], -torque.value[2]));
    //parent->ApplyTorque(-torque);
  }
}

void Thruster::Stop() {
  
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
  Thruster _rollThruster, Thruster _yawThruster, Cockpit cock)
  : hull(h),
    engine(e),
    battery(b),
    reactor(r),
    shield(s),
    rollThruster(_rollThruster),
    yawThruster(_yawThruster),
    cockpit(cock)
{}

ShipConfiguration::ShipConfiguration(const ShipConfiguration& original) 
  : hull(original.hull),
    engine(original.engine),
    battery(original.battery),
    reactor(original.reactor),
    shield(original.shield),
    rollThruster(original.rollThruster),
    yawThruster(original.yawThruster),
    cockpit(original.cockpit)
{}

void ShipConfiguration::Attach(Ship *s) {
  hull.Attach(s);
  engine.Attach(s);
  battery.Attach(s);
  reactor.Attach(s);
  shield.Attach(s);
  rollThruster.Attach(s);
  yawThruster.Attach(s);
  cockpit.Attach(s);
}

Mass ShipConfiguration::GetMass() const {
  Large mass = hull.mass.GetMass(Mass::Kilogram) + engine.mass.GetMass(Mass::Kilogram) + reactor.mass.GetMass(Mass::Kilogram);
  return Mass(mass, Mass::Kilogram);
}

Ship::Ship(Position p, Orientation o, Cylinder *s, ShipConfiguration c, Mesh *m, ShaderProgram *sp, glm::vec3 co) 
  : ControllableEntity(),
    PhysicsEntity(p, o, s, c.GetMass(), m, sp),
    config(c),
    color(co),
    dead(false)
{
  transformation->Transform(Scale(s->radius.GetLength(Length::Game)));
  config.Attach(this);
  fuelMesh = Mesh3D::FromOBJ("models/cube-test.obj");
}

Transformation *Ship::CameraAttachPoint(Camera *camera) {
  camera->BindToTransformation(transformation, config.cockpit.Sight());
  return nullptr;
}

void Ship::Update(float delta) {
  if (dead && transformation->scale.vector[0] < 300.0f) {
    transformation->Transform(Scale(1.5f));
    return;
  }

  // config.cockpit.Update(delta);
  PhysicsEntity::Update(delta);
}

void Ship::Render() {
  if (dead) {
    if (transformation->scale.vector[0] >= 300.0f) {
      return;
    }
    shader->SetColor(1, 0, 0, 0.9f);

    PhysicsEntity::Render();
    return;
  }
  shader->SetColor(color[0], color[1], color[2], 0.9f);
  PhysicsEntity::Render();
  for (int i = 0; i < fuelTrail.size(); ++i) {
    ShipFuel *f = fuelTrail[i];
    f->Render();
  }
  if (fuelTrail.size() > 8) {
    delete fuelTrail[0];
    fuelTrail.erase(fuelTrail.begin());
  }
}

void Ship::ExpelFuel() {
  fuelTrail.push_back(new ShipFuel(
    currentPosition + Displacement(glm::vec3(
      random() % 3, random() % 3, random() % 3
    ), Length::Meter),
    Length(random() % (5 - 2) + 2, Length::Meter),
    Velocity(glm::vec3(glm::vec4(glm::vec3(0.0f, 0.0f, -1.0f) * currentVelocity.scalar(Length::Meter, Time::Seconds)
      * 0.98f, 0.0f) * currentOrientation.quat())
      , Length::Meter, Time::Seconds),
    currentOrientation,
    ShipFuel::shipFuelShader,
    fuelMesh
  ));
}

bool Ship::DidCollide() {
  Mesh *temp = mesh;
  mesh = fuelMesh;
  fuelMesh = temp;
  dead = true;
  currentVelocity = glm::vec3(0.0f);
  return dead;
}

ShaderProgram *ShipFuel::shipFuelShader = nullptr;
ShipFuel::ShipFuel(Position p, Length w, Velocity v, Orientation o, ShaderProgram *sp, Mesh *m)
  : PhysicsEntity(p, o, new Point(Mass(1.0f, Mass::Kilogram), Length(1.0f, Length::Meter)), Mass(1.0f, Mass::Kilogram), m, sp),
    dead(false)
{
  color = glm::vec3(1.0f, 0.0f, 0.0f);
  transformation->Transform(Scale(w.GetLength(Length::Game)));
}

bool ShipFuel::Dead() {
  return dead;
}

void ShipFuel::Render() {
  shader->SetColor(color[0], color[1], color[2], 1.0f);
  PhysicsEntity::Render();
}