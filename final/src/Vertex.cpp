#include "ui/mesh/Vertex.h"

VertexAttribute::VertexAttribute(std::initializer_list<float> v) {
  for (float f : v) {
    value.push_back(f);
  }
}

bool VertexAttribute::operator==(const VertexAttribute& right) const {
  // False if they don't have the same value size
  if (value.size() != right.value.size()) {
    return false;
  } 
  // Then check each value
  for (int i = 0; i < value.size(); ++i) {
    if (value[i] != right.value[i]) {
      return false;
    }
  }
  return true;
}

bool Vertex::operator==(const Vertex& right) const {
  // If they don't have the same number of attributes, they are not the same
  if (attributes.size() != right.attributes.size()) {
    return false;
  }
  // If they have the same attributes, they are the same
  for (int i = 0; i < attributes.size(); ++i) {
    // Could be the same object
    if (attributes[i] == right.attributes[i]) {
      continue;
    }
    // Could be the same properties
    else if (*(attributes[i]) == *(right.attributes[i])) {
      continue;
    }
    return false;
  }
  return true;
}

VertexAttributeBuffer::VertexAttributeBuffer(const char *_name, std::vector<VertexAttribute *> attributes) 
  : length(attributes.size()),
    dimensions(attributes[0]->value.size())
{
  // Move the values of the vertex attribute into an array
  float buffer[length * dimensions];
  int i = 0;
  for (VertexAttribute *a : attributes) {
    if (a->value.size() != dimensions) {
      throw std::exception(); // All vertex attributes must be of the same dimensions
    }
    for (float f : a->value) {
      buffer[i++] = f;
    }
  }
  // Move the array into an OpenGL buffer
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, length * dimensions * sizeof(float), buffer, GL_STATIC_DRAW);
  // All done!
}

void VertexAttributeBuffer::Load(GLuint attributePointer) {
  glBindBuffer(GL_ARRAY_BUFFER, id); // Bind the buffer
  // Bind the attribute pointer
  glVertexAttribPointer(attributePointer, dimensions, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glEnableVertexAttribArray(attributePointer);
}

VertexAttributeBuffer::~VertexAttributeBuffer() {
  glDeleteBuffers(1, &id);
}

VertexArray::VertexArray(std::vector<Vertex *> vertices) 
  : length(vertices.size())
{
  // Initialize and bind VAO
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Get number of attributes for each vertex
  int nOfAttributes = vertices[0]->attributes.size();
  // And now, create a vector to store the attributes at the correct index
  std::vector<std::vector<VertexAttribute *> > attributes;
  for (int i = 0; i < nOfAttributes; ++i) {
    attributes.push_back(std::vector<VertexAttribute *>());
  }

  // Create an array to store indices for each vertex
  unsigned short elementArray[length];

  std::vector<Vertex *> added; // Keep track of unique vertices

  for (int i = 0; i < vertices.size(); ++i) { // For each new vertex
    Vertex *v = vertices[i];
    if (v->attributes.size() != nOfAttributes) {
      throw std::exception(); // Each vertex needs the same # of attributes!
    }
    bool duplicate = 0;
    unsigned short elementIndex = 0;
    for (Vertex *a : added) { // Check if there is a vertex already added
      if (*v == *a) {
        // There is already a vertex with same attributes, it's a duplicate
        duplicate = true;
        break;
      }
      ++elementIndex; // Otherwise, keep searching
    }
    // If it was a duplicate, don't add to added
    if (!duplicate) {
      added.push_back(v);
      elementIndex = added.size() - 1; // The element index is the last added one
    }
    // std::cout << elementIndex << std::endl;
    elementArray[i] = elementIndex; // add index to the array
  }
  
  // So now we've added all the vertices, create the AttributeBuffers
  // First, assemble all the attributes of the unique vertices
  for (Vertex *v : added) {
    for (int i = 0; i < nOfAttributes; ++i) {
      attributes[i].push_back(v->attributes[i]);
    }
  }
  // Next create the buffers
  for (int i = 0; i < nOfAttributes; ++i) {
    attributeArrays.push_back(new VertexAttributeBuffer(nullptr, attributes[i]));
  }

  // Generate the element buffer in OpenGL
  glGenBuffers(1, &id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof(unsigned short), elementArray, GL_STATIC_DRAW);
  // We should be done
}

VertexArray::~VertexArray() {
  glDeleteBuffers(1, &id);
  for (VertexAttributeBuffer *b : attributeArrays) {
    delete b;
  }
  glDeleteVertexArrays(1, &vao);
}

void VertexArray::Draw(std::initializer_list<int> attributes) {
  std::vector<int> lst;
  for (int id : attributes) {
    lst.push_back(id);
  }
  Draw(lst); // For consistency
}

void VertexArray::Draw(std::vector<int> attributes) {
  // For each attribute, enable an attribute buffer,
  // if -1 is passed, skip enabling that attribute
  glBindVertexArray(vao);
  int i = -1;
  for (int d : attributes) {
    ++i; // Gotta increment no matter what
    if (d < 0) continue;
    attributeArrays[i]->Load(d);
  }
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); // Bind the element array
  glDrawElements(
    GL_TRIANGLES,        // mode
    length,              // count
    GL_UNSIGNED_SHORT,   // type
    (void*)0             // element array buffer offset
  );
  for (int d : attributes) {
    glDisableVertexAttribArray(d); // disable attribute arrays when done drawing
  }
}