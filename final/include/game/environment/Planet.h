#ifndef PLANET_DEF
#define PLANET_DEF

#include "standard.h"
#include "physics/entity/PhysicsEntity.h"
#include "ui/mesh/Mesh3D.h"
#include "ui/transformation/Transformation.h"
#include "physics/properties/Mass.h"
#include "physics/properties/Length.h"
#include "ui/program/Program.h"

class Planet : public PhysicsEntity {
public: 
  Planet(Position position, Mass m, Length r, Mesh3D *mesh, ShaderProgram *shader); // TODO: Make 3D objects ONLY accept uniform transformations

};

#endif