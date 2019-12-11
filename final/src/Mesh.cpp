#include "ui/mesh/Mesh.h"

Mesh::Mesh(int _dimensions, VertexArray *vertexArray)
  : vertices(vertexArray),
    dimensions(_dimensions),
    numVertices(vertexArray->length)
{
  // glGenVertexArrays(1, &vao);
}

Mesh::Mesh(int _dimensions, std::initializer_list<float> vertices)
  : dimensions(_dimensions),
    textured(false),
    numVertices(vertices.size() / _dimensions)
{
  
}

Mesh::Mesh(int _dimensions, std::initializer_list<float> verts, std::initializer_list<float> texture)
  : dimensions(_dimensions),
    textured(true),
    numVertices(verts.size() / _dimensions)
{
  std::vector<VertexAttribute *> v;
  std::vector<VertexAttribute *> uv;
  std::vector<float> buff;
  for (float f : verts) {
    buff.push_back(f);
    if (buff.size() == dimensions) {
      v.push_back(new VertexAttribute({buff[0], buff[1], buff[2]}));
      buff.clear();
    }
  }
  buff.clear();
  for (float f : texture) {
    buff.push_back(f);
    if (buff.size() == 2) {
      uv.push_back(new VertexAttribute({buff[0], buff[1]}));
      buff.clear();
    }
  }
  std::vector<Vertex *> faces;
  for (int i = 0; i < numVertices; ++i) {
    Vertex *newV = new Vertex();
    newV->attributes.push_back(v[i]);
    newV->attributes.push_back(uv[i]);
    faces.push_back(newV);
  }
  VertexArray *array = new VertexArray(faces);
  vertices = array;
  // Free unneeded stuff
  for (Vertex *v : faces) delete v;
  for (VertexAttribute *v : v) delete v;
  for (VertexAttribute *v : uv) delete v;
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
