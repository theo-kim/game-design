#ifndef SHAPES_DEF
#define SHAPES_DEF

#include "standard.h"
#include "Mass.h"
#include "Length.h"

class Shape { 
public:
  enum Axis { X, Y, Z };
  Shape(Mass m, Length r) : mass(m), radius(r) {}
  virtual float GetMoment(Axis a) { return 0.0f; };
  Mass mass;
  Length radius;
};

class Point : public Shape {
public:
  Point(Mass m, Length r) : Shape(m, r) {}
  virtual float GetMoment(Axis a) { 
    if (a == Z) return mass.GetMass() * pow(radius.GetLength(), 2);
    return 0.0f;
  }
};

class Cylinder : public Shape {
public:
  Cylinder(Mass m, Length r, Length h) : Shape(m, r), height(h) {}
  Length height;
  virtual float GetMoment(Axis a) { 
    if (a == Z) return 0.5f * mass.GetMass() * pow(radius.GetLength(), 2);
    return 0.0833f * mass.GetMass() * (3 * pow(radius.GetLength(), 2) + pow(height.GetLength(), 2));
  }
};

class EndRod : public Shape {
public:
  EndRod(Mass m, Length l) : Shape(m, l) {}
  virtual float GetMoment(Axis a) { 
    if (a == Z) return 0.333f * mass.GetMass() * pow(radius.GetLength(), 2);
    return 0.0f;
  }
};

class CenterRod : public Shape {
public:
  CenterRod(Mass m, Length l) : Shape(m, l) {}
  virtual float GetMoment(Axis a) { 
    if (a == Z) return 0.0833f * mass.GetMass() * pow(radius.GetLength(), 2);
    return 0.0f;
  }
};

class Sphere : public Shape {
public:
  Sphere(Mass m, Length r) : Shape(m, r) {}
  virtual float GetMoment(Axis a) { return 0.4f * mass.GetMass() * pow(radius.GetLength(), 2); }
};

#endif