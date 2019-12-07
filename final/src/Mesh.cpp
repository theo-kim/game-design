#include "ui/mesh/Mesh.h"

Mesh::Mesh(int _dimensions, VertexArray *vertexArray)
  : vertices(vertexArray),
    dimensions(_dimensions),
    numVertices(vertexArray->length)
{
  // glGenVertexArrays(1, &vao);
}

Mesh::Mesh(const Mesh &original) 
  : dimensions(original.dimensions),
    numVertices(original.numVertices),
    vertices(original.vertices)
{
  
}

Mesh::~Mesh() {
  delete vertices;
}
