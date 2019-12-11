#include "ui/transformation/Transformation.h"

// Translation
Transformation::Translation::Translation() : vector(0.0f) {}
Transformation::Translation::Translation(float s) : vector(s) {}
Transformation::Translation::Translation(float x, float y, float z) : vector(x, y, z) {}
Transformation::Translation::Translation(glm::vec3 _vector) : vector(_vector) {}

glm::mat4 Transformation::Translation::GetMatrix() const {
  return glm::translate(glm::mat4(1.0f), vector);
}
// Convert into a model matrix
Transformation::Translation::operator glm::mat4 () const {
  return GetMatrix();
}
void Transformation::Translation::operator += (Translation& right) {
  vector = vector + right.vector;
}
Transformation::Translation Transformation::Translation::operator + (Translation& right) const {
  glm::vec3 newVector = vector + right.vector;
  Translation newTranslation(newVector);
  return newTranslation;
}
Transformation::Translation Transformation::Translation::operator- () const {
  return Translation(vector * -1.0f);
}
// End Translation

// Rotation
Transformation::Rotation::Rotation() : x(0), y(0), z(0) {}
Transformation::Rotation::Rotation(float _z) : x(0), y(0), z(_z) {}
Transformation::Rotation::Rotation(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

glm::mat4 Transformation::Rotation::GetMatrix() const {
  return glm::rotate(glm::mat4(1.0f), x, glm::vec3(1, 0, 0)) * 
         glm::rotate(glm::mat4(1.0f), y, glm::vec3(0, 1, 0)) * 
         glm::rotate(glm::mat4(1.0f), z, glm::vec3(0, 0, 1));
}
Transformation::Rotation::operator glm::mat4 () const {
  return GetMatrix();
} // Convert into a model matrix
void Transformation::Rotation::operator += (Rotation& right) {
  x += right.x;
  y += right.y;
  z += right.z;
}
Transformation::Rotation Transformation::Rotation::operator + (Rotation& right) const {
  Rotation sum;
  sum.x = x + right.x;
  sum.y = y + right.y;
  sum.z = z + right.z;
  return sum;
}
Transformation::Rotation Transformation::Rotation::operator-() const {
  return Rotation(-x, -y, -z);
}
// End Rotation

// Scale
Transformation::Scale::Scale() : vector(1.0f) {}
Transformation::Scale::Scale(float s) : vector(s) {}
Transformation::Scale::Scale(float x, float y, float z) : vector(x, y, z) {}
Transformation::Scale::Scale(glm::vec3 v) : vector(v) {}

glm::mat4 Transformation::Scale::GetMatrix() const {
  return glm::scale(glm::mat4(1.0f), vector);
}
Transformation::Scale::operator glm::mat4 () const {
  return GetMatrix();
} // Convert into a model matrix
Transformation::Scale Transformation::Scale::operator- () const {
  return Scale(vector * -1.0f);
}
void Transformation::Scale::operator += (Scale& right) {
  vector = vector + right.vector;
}
Transformation::Scale Transformation::Scale::operator + (Scale& right) const {
   Scale sum(vector + right.vector);
   return sum;
}
void Transformation::Scale::operator *= (float scalar) {
  vector = vector * scalar;
}
Transformation::Scale Transformation::Scale::operator * (float scalar) const {
  Scale product(vector * scalar);
  return product;
}
// End Scale

Transformation::Transformation()
  : translation(),
    scale(),
    rotation()
{}

Transformation::Transformation(Translation _pos, Scale _size, Rotation _rot)
  : translation(_pos),
    scale(_size),
    rotation(_rot)
{}

// Return model matrix for the transformation
glm::mat4 Transformation::GetModelMatrix() const {
  return translation.GetMatrix() * rotation.GetMatrix() * scale.GetMatrix();
}

glm::mat4 Transformation::GetGlobalModelMatrix() const {
  return scale.GetMatrix() * rotation.GetMatrix() * glm::translate(glm::mat4(1.0f), translation.vector * -1.0f);
}

Transformation::operator glm::mat4() const {
  return GetModelMatrix();
}

glm::mat4 Transformation::operator* (const Transformation& right) const {
  return GetModelMatrix() * right.GetModelMatrix();
}

glm::mat4 Transformation::operator* (const glm::mat4& right) const {
  return GetModelMatrix() * right;
}

void Transformation::Transform(Translation t) {
  translation += t;
}

void Transformation::Transform(Rotation r) {
  rotation += r;
}

void Transformation::Transform(Scale s) {
  scale += s;
}

void Transformation::LocalTransform(Translation t) {
  glm::quat q = glm::quat(glm::vec3(rotation.x, rotation.y, rotation.z));
  Translation tnew = Translation(glm::vec3(q * glm::vec4(t.vector, 0.0f)));
  translation += tnew;
}

void Transformation::LocalInverseTransform(Translation t) {
  glm::quat q = glm::quat(glm::vec3(-rotation.x, -rotation.y, -rotation.z));
  Translation tnew = Translation(glm::vec3(q * glm::vec4(t.vector, 0.0f)));
  translation += tnew;
}