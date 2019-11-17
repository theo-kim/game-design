#ifndef ENTITY_DEF
#define ENTITY_DEF

#include "standard.h"

// THIS IS AN ABSTRACT CLASS
class Entity {
 public:
  Entity();
  Entity(ShaderProgram* _program);
  Entity(ShaderProgram* _program, glm::vec3 _pos, glm::vec3 _size, float _rot);
  ~Entity();

  // Abstract Functions
  virtual void Render() = 0;
  virtual void Update(float delta) = 0;

  virtual void Render(glm::mat4 modelMatrix, GLuint texture, float* map, int points) const;
  bool CheckRenderBounds(float left, float right, float top, float bottom);
  bool CheckBounds(float left, float right, float top, float bottom);
  void SetRenderFlag(bool val);
  void SetTransformation(glm::vec3 _pos, glm::vec3 _size, float _rot);

  // Getters
  glm::vec3 GetPos () const;
  glm::vec3 GetSize () const;
  float GetRot () const;
  glm::mat4 GetCorners () const;

  // Setters
  void AddPos(glm::vec3 _addition);
  void SetPos(glm::vec3 _pos);
  void ScaleSize(glm::vec3 _scale);
  void SetSize(glm::vec3 _size);
  void AddRot(float _addition);
  void SetRot(float _rot);

  // Utility functions to transform member bodies / points
  void TransformLocalCoord (glm::vec3 &point) const;
  void TransformLocalCoord (glm::mat4 &body) const;
  float TransformLocalRot (float rot) const;
private:
  // Transformation variables
  glm::vec3 pos;
  glm::vec3 size;
  float rot;

  // Render resources
  ShaderProgram* program;
  
  // Render state
  bool doRender;
};

class TexturedEntity : public Entity {
public:
  TexturedEntity();
  TexturedEntity(ShaderProgram* r, TextureSheet *texture);
  TexturedEntity(ShaderProgram* r, TextureSheet *texture, glm::vec3 _pos, glm::vec3 _size, float _rot);
  ~TexturedEntity();

  void Render(glm::mat4 modelMatrix, float* map, int points) const;

  // Getter
  TextureSheet *GetTexture() const;

  // Setter
  void SetTexture(TextureSheet *newTexture);

private:
  TextureSheet *texture;
};

class UntexturedEntity : public Entity {
public:
  UntexturedEntity();
  UntexturedEntity(ShaderProgram* r, glm::vec3 color);
  UntexturedEntity(ShaderProgram* r, glm::vec3 color, glm::vec3 _pos, glm::vec3 _size, float _rot);
  ~UntexturedEntity();

  void Render(glm::mat4 modelMatrix, float* map, int points) const;

  // Getter
  glm::vec3 GetColor() const;

  // Setter
  void SetColor(glm::vec3 newColor);

private:
  glm::vec3 color;
};

#endif
