#ifndef ENTITY_DEF
#define ENTITY_DEF

#include "standard.h"
#include "ui/mesh/Mesh.h"
#include "ui/transformation/Transformation.h"
#include "ui/program/Program.h"
#include "ui/camera/Camera.h"

class Camera;
class Mesh;
class ShaderProgram;

class Entity {
public:
  // Constructors
  Entity(Mesh *mesh, Transformation *transformation, ShaderProgram *shader);

  // Abstract functions
  virtual void Render() const;
  virtual void Update(float delta);
  virtual void Input();

  friend class Camera;
protected:
  Transformation *transformation;
private: 
  Mesh *mesh;
  ShaderProgram *shader;
};

#endif