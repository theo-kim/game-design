#ifndef MINIMAP_DEF
#define MINIMAP_DEF

#include "standard.h"
#include "Entity.h"

class MiniMap {
public:
  MiniMap();
  MiniMap(ShaderProgram *renderer, glm::vec3 center, glm::vec3 size, glm::vec3 projection);
  void Update(std::vector<Entity *> map);
  void MapColor(Entity *target, glm::vec3 color);
private:
  class Marker : public Entity {
  public:
    Marker(ShaderProgram *renderer, glm::vec3 pos);
  };

  float width, height;
  float left, right, top, bottom;
  std::map<Entity *, glm::vec3> colorMapping;
  ShaderProgram *renderer;
};

#endif
