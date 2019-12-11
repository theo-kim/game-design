#include "game/environment/Planet.h"
#include "game/environment/Sun.h"

// Planet

Planet::Planet(Position position, Mass m, Length r, Mesh3D *mesh, ShaderProgram *shader) 
  : PhysicsEntity(
      position, 
      Orientation(glm::vec3(0.0f)),
      Sphere(m, r),
      m,
      mesh,
      shader
    )
{
  transformation->Transform(Transformation::Scale(r.GetLength(Length::Game) * 2.0f));
}

// Sun

Sun::Sun(Position position, Mass m, Length r, Power power, Mesh3D *mesh, 
  ShaderProgram *shader, glm::vec3 color)
  : PhysicsEntity(position, Orientation(glm::vec3(0.0f)), Sphere(m, r), m, mesh, shader),
    light(transformation, power.GetPower(Energy::Joule, Time::Seconds), color)
{
  transformation->Transform(Transformation::Scale(r.GetLength(Length::Game) * 2.0f));
}

Light& Sun::GetLight() {
  return light;
}