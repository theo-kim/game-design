#ifndef SHIP_DEF
#define SHIP_DEF

#include "standard.h"
#include "ui/entity/Entity.h"
#include "ui/transformation/Transformation.h"

class Ship : public Entity {
public:
  Ship(Transformation *t);
};

#endif