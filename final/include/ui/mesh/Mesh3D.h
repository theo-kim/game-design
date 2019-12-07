#ifndef MESH3D_DEF
#define MESH3D_DEF

#include <fstream>
#include <sstream>

#include "standard.h"
#include "ui/program/Program.h"
#include "ui/mesh/Mesh.h"
#include "ui/mesh/Vertex.h"

class Mesh3D : public Mesh {
public:
  // Constructors
  Mesh3D(VertexArray *array);
  // Mesh3D(std::initializer_list<float> _vertices, std::initializer_list<float> _normals);
  // Mesh3D(std::initializer_list<float> _vertices, std::initializer_list<float> _normals, std::initializer_list<float> _texture);
  // // Mesh iterable definition
  // Mesh3D(std::vector<float> _vertices, std::vector<float> _normals);
  // Mesh3D(std::vector<float> _vertices, std::vector<float> _normals, std::vector<float> _texture);
  // Copy constructor
  Mesh3D(const Mesh3D &original);
  // Destructor
  virtual ~Mesh3D();

  // Factory
  static Mesh3D *FromOBJ(const char * filepath);

  // Abstracted functions
  virtual void Render(ShaderProgram *shader) const;
};

#endif