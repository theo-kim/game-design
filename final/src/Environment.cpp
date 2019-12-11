#include "game/environment/Planet.h"
#include "game/environment/Sun.h"
#include "game/environment/Asteroid.h"

// Planet

Planet::Planet(Position position, Mass m, Length r, Mesh3D *mesh, ShaderProgram *shader) 
  : PhysicsEntity(
      position, 
      Orientation(glm::vec3(0.0f)),
      new Sphere(m, r),
      m,
      mesh,
      shader
    )
{
  transformation->Transform(Transformation::Scale(r.GetLength(Length::Game) * 2.0f));
}

void Planet::Render() {
  shader->SetColor(0.0f, 0.4f, 0.9f, 1.0f);
  PhysicsEntity::Render();
}

// Sun

Sun::Sun(Position position, Mass m, Length r, Power power, Mesh3D *mesh, 
  ShaderProgram *shader, glm::vec3 color)
  : PhysicsEntity(
      position, 
      Orientation(glm::vec3(0.0f)), 
      new Sphere(m, r), 
      m, 
      mesh, 
      shader),
    light(transformation, power.GetPower(Energy::Joule, Time::Seconds), color)
{
  transformation->Transform(Transformation::Scale(r.GetLength(Length::Game) * 2.0f));
}

Light& Sun::GetLight() {
  return light;
}

Asteroid::Asteroid(Position p, Mass m, Length r, AngularVelocity a, Velocity v, Mesh *mess, ShaderProgram *sp)
  : PhysicsEntity(
      p, 
      Orientation(glm::vec3(0.0f)),
      new Sphere(m, r),
      m,
      mess, 
      sp
    )
{
  currentVelocity = v;
  currentAngularVelocity = a;
  transformation->Transform(Transformation::Scale(r.GetLength(Length::Game) * 2.0f));
}

void Asteroid::Render() {
  shader->SetColor(0.5f, 0.5f, 0.5f, 1.0f);
  PhysicsEntity::Render();
}

Station::Station(Position p, Mass m, Length r, Mesh *mess, ShaderProgram *sp)
  : PhysicsEntity(
      p, 
      Orientation(glm::vec3(radians(10.0f), 0.0f, radians(10.0f))),
      new Sphere(m, r),
      m,
      mess, 
      sp
    )
{
  currentAngularVelocity = AngularVelocity(glm::vec3(0,0, radians(10.0f)), Time::Seconds);
  transformation->Transform(Transformation::Scale(r.GetLength(Length::Game) * 2.0f));
}

void Station::Render() {
  shader->SetColor(0.95f, 0.95f, 0.95f, 1.0f);
  PhysicsEntity::Render();
}