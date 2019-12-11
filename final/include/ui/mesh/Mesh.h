#ifndef MESH_DEF
#define MESH_DEF

#include "standard.h"
#include "ui/program/Program.h"
#include "ui/mesh/Vertex.h"

class ShaderProgram;

class Mesh {
public:
  // Mesh VertexArray definition
  Mesh(int _dimensions, VertexArray *vertexArray);
  Mesh(int _dimensions, std::initializer_list<float> vertices);
  Mesh(int _dimensions, std::initializer_list<float> vertices, std::initializer_list<float> uv);
  // Copy constructor
  Mesh(const Mesh &original);
  // Destructor
  virtual ~Mesh();

  // Abstract methods
  virtual void Render(ShaderProgram *shader) const = 0;
protected:
  int numVertices;
  // VAO identifier needs to be accessible to descedents
  GLuint vao;
  VertexArray *vertices;
  bool textured;
  // VertexArray * VertexArrayFromList(std::vector<) const;
private:
  int dimensions;
};

#endif