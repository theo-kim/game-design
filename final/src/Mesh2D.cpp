#include "ui/mesh/Mesh2D.h"

// Mesh2D::Mesh2D(std::initializer_list<float> _vertices)
//   : Mesh(2, _vertices) 
// {}

// Mesh2D::Mesh2D(std::initializer_list<float> _vertices, std::initializer_list<float> _texture) 
//   : Mesh(2, _vertices, _texture)
// {}

// Mesh2D::Mesh2D(std::vector<float> _vertices)
//   : Mesh(2, _vertices)
// {}

// Mesh2D::Mesh2D(std::vector<float> _vertices, std::vector<float> _texture) 
//   : Mesh(2, _vertices, _texture)
// {}

// Copy constructor
Mesh2D::Mesh2D(const Mesh2D &original)
  : Mesh(original)
{}

Mesh2D::~Mesh2D() {}

// Instance of a square mesh for many 2D entity use cases
// Mesh2D *Mesh2D::Square = new Mesh2D({ -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 });

// Render 2D mesh to the screen
void Mesh2D::Render(ShaderProgram *shader) const {
  // Mesh::Render(shader);
}