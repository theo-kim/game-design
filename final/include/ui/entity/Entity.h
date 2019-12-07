#ifndef ENTITY_DEF
#define ENTITY_DEF

#include "standard.h"
#include "ui/mesh/Mesh.h"
#include "ui/transformation/Transformation.h"
#include "ui/program/Program.h"

class Entity {
public:
  // Constructors
  Entity(Mesh *mesh, Transformation *transformation, ShaderProgram *shader);

  // Abstract functions
  virtual void Render() const;
  virtual void Update(float delta);
  virtual void Input();
protected:
  Transformation *transformation;
private: 
  Mesh *mesh;
  ShaderProgram *shader;
};

#endif