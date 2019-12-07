#ifndef PLANET_DEF
#define PLANET_DEF

#include "standard.h"
#include "ui/entity/Entity.h"
#include "ui/mesh/Mesh3D.h"
#include "ui/transformation/Transformation.h"
#include "physics/properties/Mass.h"
#include "physics/properties/Length.h"
#include "ui/program/Program.h"

class Planet : public Entity{
public: 
  static const char *meshName;
  Planet(Transformation::Translation position, Mass m, Length r, ShaderProgram *shader); // TODO: Make 3D objects ONLY accept uniform transformations
private:
  Mass mass;
  Length radius;
};

#endif