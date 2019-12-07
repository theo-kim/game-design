#ifndef FORCED_DEF
#define FORCED_DEF

#include "standard.h"
#include "ui/transformation/Transformation.h"
#include "physics/properties/Mass.h"
#include "physics/properties/Force.h"

class Forced : public Transformation {
public:
  Forced(Transformation initial, Mass mass);
  void ApplyForce(Force f);
};

#endif