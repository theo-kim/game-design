#ifndef SUN_DEF
#define SUN_DEF

#include "standard.h"
#include "physics/entity/PhysicsEntity.h"
#include "physics/properties/Energy.h"
#include "ui/mesh/Mesh3D.h"
#include "ui/light/Light.h"

class PhysicsEntity;

class Sun : public PhysicsEntity {
public:
  Sun(Position position, Mass m, Length r, Power power, Mesh3D *mesh, ShaderProgram *shader, glm::vec3 color);
  Light& GetLight();
private:
  Light light;
};

#endif