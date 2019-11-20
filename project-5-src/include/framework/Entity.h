#ifndef ENTITY_DEF
#define ENTITY_DEF

#include "gl.h"

// THIS IS AN ABSTRACT CLASS
class Entity {
 public:
  Entity();
  Entity(ShaderProgram* _program);
  Entity(ShaderProgram* _program, glm::vec3 _pos, glm::vec3 _size, float _rot);
  virtual ~Entity();

  // Abstract Functions
  virtual void Render() = 0;
  virtual void Update(float delta) = 0;

  void Render(glm::mat4 modelMatrix, GLuint texture, float* map, int points) const;
  bool CheckRenderBounds(float left, float right, float top, float bottom);
  bool CheckBounds(float left, float right, float top, float bottom);
  void SetRenderFlag(bool val);
  void SetTransformation(glm::vec3 _pos, glm::vec3 _size, float _rot);

  // Getters
  virtual glm::vec3 GetPos () const;
  glm::vec3 GetSize () const;
  float GetRot () const;
  glm::mat4x3 GetCorners () const;
  ShaderProgram* GetProgram() const;
  virtual bool GetGarbage() const;

  // Setters
  virtual void AddPos(glm::vec3 _addition);
  void SetPos(glm::vec3 _pos);
  void ScaleSize(glm::vec3 _scale);
  void SetSize(glm::vec3 _size);
  void AddRot(float _addition);
  void SetRot(float _rot);
  virtual void SetGarbage();

  // Utility functions to transform member bodies / points
  void TransformLocalCoord (glm::vec3 &point) const;
  void TransformLocalCoord (glm::mat4 &body) const;
  void TransformToLocalCoord (glm::vec3 &point) const;
  float TransformLocalRot (float rot) const;

private:
  // Clean up
  bool killme;
  
  // Transformation variables
  glm::vec3 pos;
  glm::vec3 size;
  float rot;

  // Render resources
  ShaderProgram* program;
  
  // Render state
  bool doRender;
};

class TexturedEntity : virtual public Entity {
public:
  TexturedEntity();
  TexturedEntity(TexturedShader* r, TextureSheet *texture);
  TexturedEntity(TexturedShader* r, TextureSheet *texture, glm::vec3 _pos, glm::vec3 _size, float _rot);
  virtual ~TexturedEntity();

  void Render(glm::mat4 modelMatrix, float* map, int points) const;

  // Getter
  TextureSheet *GetTexture() const;

  // Setter
  void SetTexture(TextureSheet *newTexture);

private:
  TextureSheet *texture;
};

class UntexturedEntity : virtual public Entity {
public:
  UntexturedEntity();
  UntexturedEntity(UntexturedShader* r, glm::vec3 color);
  UntexturedEntity(UntexturedShader* r, glm::vec3 color, glm::vec3 _pos, glm::vec3 _size, float _rot);
  virtual ~UntexturedEntity();

  void Render(glm::mat4 modelMatrix, float* map, int points) const;

  // Getter
  glm::vec3 GetColor() const;

  // Setter
  void SetColor(glm::vec3 newColor);

private:
  glm::vec3 color;
};

// Templated Class for Holding Entities
template <class T>
class EntityGroup : virtual public Entity {
public:
  EntityGroup() {};
  EntityGroup(std::initializer_list<T *> l) { for (T *x : l) children.push_back(x); };

  ~EntityGroup() { for (int i = 0; i < children.size(); ++i) delete children[i]; };

  // Getters
  T *GetEntity(int index) const { return children[index]; };
  int GetNumChildren() const { return children.size(); };

  // Setter
  void Empty() { children.clear(); }

  // Adder
  void AddEntity(T *newEntity) { children.push_back(newEntity); };
private:
  std::vector<T *> children;
};

class Container : public EntityGroup<Entity> {
public: 
  Container(std::initializer_list<Entity *> entities);
  
  virtual void Render();

  virtual void Update(float delta);

  // Adder
  virtual void AddPos(glm::vec3 add);
};

#endif
