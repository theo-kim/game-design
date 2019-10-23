#ifndef SHIP_DEF
#define SHIP_DEF

#include "standard.h"
#include "Entity.h"
#include "Sensor.h"
#include "ProximitySensor.h"
#include "EdgeSensor.h"
#include "VertexSensor.h"

class Ship : public Entity {
public:
  Ship();
  Ship(ShaderProgram* program);
  Ship(ShaderProgram* program, glm::vec3 p, glm::vec3 s);
  void Render();
  void Update(float delta);
  void Move(bool up, bool down, bool left, bool right);

  glm::vec3 GetMov() const;
  
  // I'm lazy and putting the sensors in the public space
  Sensor *sensors[9]; // For now...
  ProximitySensor prox;
  EdgeSensor edges[4];
  VertexSensor verts[4];
  
private:
  int health;
  glm::vec3 mov;
  float speed;
};

#endif
