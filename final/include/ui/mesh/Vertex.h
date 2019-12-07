#ifndef VERTEX_DEF
#define VERTEX_DEF

#include "standard.h"

class VertexAttribute {
public:
  VertexAttribute(std::initializer_list<float> values);
  std::vector<float> value;
  bool operator==(const VertexAttribute& right) const;
};

class VertexAttributeBuffer {
public:
  VertexAttributeBuffer(const char *name, std::vector<VertexAttribute *> attributes);
  ~VertexAttributeBuffer(); // To clear out OpenGL buffers
  void Load(GLuint attributePointer);
private:
  const char *name;
  int length;
  int dimensions;
  GLuint id;
};

class Vertex {
public:
  std::vector<VertexAttribute *> attributes;
  bool operator==(const Vertex& right) const;
};

class VertexArray {
public:
  VertexArray(std::vector<Vertex *> vertices);
  ~VertexArray(); // To clear out OpenGL buffers
  void Draw(std::initializer_list<int> attributes);
  void Draw(std::vector<int> attributes);
  std::vector<VertexAttributeBuffer *> attributeArrays;
  int length;
private:
  GLuint id;
  GLuint vao;
};

#endif