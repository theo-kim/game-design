#ifndef MESH2D_DEF
#define MESH2D_DEF

#include "standard.h"
#include "ui/mesh/Mesh.h"

class Mesh2D : public Mesh {
public:
  // Constructors
  Mesh2D(std::initializer_list<float> _vertices);
  Mesh2D(std::initializer_list<float> _vertices, std::initializer_list<float> _texture);
  // // Mesh iterable definition
  // Mesh2D(std::vector<float> _vertices);
  // Mesh2D(std::vector<float> _vertices, std::vector<float> _texture);
  // Copy constructor
  Mesh2D(const Mesh2D &original);
  // Destructor
  virtual ~Mesh2D();

  // Useful instances
  static Mesh2D *Square;

  // Abstracted functions
  virtual void Render(ShaderProgram *shader) const;
};

#endif