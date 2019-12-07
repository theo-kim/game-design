#include "game/environment/Planet.h"

const char *Planet::meshName = "models/sphere-smooth-test.obj";

Planet::Planet(Transformation::Translation position, Mass m, Length r, ShaderProgram *shader) 
  : Entity(
      Mesh3D::FromOBJ(meshName), 
      new Transformation(position, Transformation::Scale(1.0f), Transformation::Rotation()),
      shader
    ),
    mass(m),
    radius(r)
{
  transformation->Transform(Transformation::Scale(radius.GetLength(Length::Game)));
}