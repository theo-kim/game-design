#ifndef EDGE_DEF
#define EDGE_DEF

#include "../gl.h"
#include "Sensor.h"
#include "VertexSensor.h"

class VertexSensor;

class EdgeSensor : public Sensor {
public:
  EdgeSensor();
  EdgeSensor(glm::vec3 _boundsLeft, glm::vec3 _boundsRight, Entity *belongsTo);
  bool CheckCollision(EdgeSensor sensor);
  bool CheckCollision(VertexSensor sensor);
  
  virtual int GetState();
  virtual void Update(glm::mat4 &transformation);
  glm::vec3 boundsLeft;
  glm::vec3 boundsRight;
  float m;
  float A;
  float B;
  float C;
};

#endif
