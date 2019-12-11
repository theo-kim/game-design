#ifndef STATION_DEF
#define STATION_DEF

#include "physics/entity/PhysicsEntity.h"

class Station : public PhysicsEntity {
public:
  Station(Position p, Mass m, Length r, Mesh *mesh, ShaderProgram *sp);
  void Render();
  // friend class CollisionController;
};

#endif