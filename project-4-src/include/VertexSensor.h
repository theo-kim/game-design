#ifndef VERTEXSENSOR_DEF
#define VERTEXSENSOR_DEF

#include "standard.h"
#include "Sensor.h"
#include "EdgeSensor.h"

class EdgeSensor;

class VertexSensor : public Sensor {
public:
  VertexSensor();
  VertexSensor(glm::vec3 _loc, Entity *belongsTo);
  bool CheckCollision(VertexSensor sensor);
  bool CheckCollision(EdgeSensor sensor);
  virtual int GetState();
  virtual void Update(glm::mat4 &transformation);
  glm::vec3 GetLoc();
private:
  glm::vec3 loc;
};

#endif
