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
  void Render();
  void Update(float delta);

  // I'm lazy and putting the sensors in the public space
  Sensor *sensors[9]; // For now...
  ProximitySensor prox;
  EdgeSensor edges[4];
  VertexSensor verts[4];
  
private:
  int health;
};

#endif
