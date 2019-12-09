#include "game/environment/Planet.h"

Planet::Planet(Position position, Mass m, Length r, Mesh3D *mesh, ShaderProgram *shader) 
  : PhysicsEntity(
      position, 
      Orientation(glm::vec3(0.0f)),
      Sphere(m, r),
      m,
      mesh,
      shader
    ),
    mass(m),
    radius(r)
{
  transformation->Transform(Transformation::Scale(r.GetLength(Length::Game)));
}