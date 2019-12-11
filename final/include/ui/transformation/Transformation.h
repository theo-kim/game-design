#ifndef TRANSFORMATION_DEF
#define TRANSFORMATION_DEF

#include "standard.h"

class Transformation {
public:
  // Member classes for specific aspects of a Transformation
  class Translation {
  public: 
    Translation();
    Translation(float s);
    Translation(float x, float y, float z);
    Translation(glm::vec3 _vector);
    
    glm::mat4 GetMatrix() const;
    operator glm::mat4 () const; // Convert into a model matrix
    Translation operator-() const;
    void operator += (const Translation& right);
    Translation operator + (const Translation& right) const;
    glm::vec3 vector;
  };
  class Rotation {
  public: 
    Rotation();
    Rotation(float z);
    Rotation(float x, float y, float z);
    
    glm::mat4 GetMatrix() const;
    operator glm::mat4 () const; // Convert into a model matrix
    Rotation operator-() const;
    void operator += (const Rotation& right);
    Rotation operator + (const Rotation& right) const;
    float x, y, z;
  };
  class Scale {
  public: 
    Scale();
    Scale(float s);
    Scale(float x, float y, float z);
    Scale(glm::vec3 v);
    
    glm::mat4 GetMatrix() const;
    operator glm::mat4 () const; // Convert into a model matrix
    Scale operator-() const;
    void operator += (const Scale& right);
    Scale operator + (const Scale& right) const;
    void operator *= (float scalar);
    Scale operator * (float scalar) const;
    glm::vec3 vector;
  };
  
  // Constructors
  Transformation();
  Transformation(Translation pos, Scale size, Rotation rot);
  // Using default copy constructor

  virtual void Transform(Translation t);
  virtual void Transform(Rotation r);
  virtual void Transform(Scale s);

  virtual void LocalTransform(Translation t);
  
  // For use with camera objects only
  virtual void LocalInverseTransform(Translation t);

  // Getters
  virtual glm::mat4 GetGlobalModelMatrix() const;
  virtual glm::mat4 GetModelMatrix() const;
  virtual operator glm::mat4() const; // Convert into a model matrix implictly

  // Operator overloads
  virtual glm::mat4 operator *(const Transformation& right) const;
  virtual glm::mat4 operator *(const glm::mat4& right) const;
  virtual Transformation operator +(const Transformation& right) const;

  Translation translation;
  Scale scale;
  Rotation rotation;
};

typedef Transformation::Scale Scale;
typedef Transformation::Translation Translation;
typedef Transformation::Rotation Rotation;

#endif