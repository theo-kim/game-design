#ifndef ENERGY_DEF
#define ENERGY_DEF

#include "Time.h"

class Energy {
public:
  enum Unit { Joule, Kilojoule = 1000, Megajoule = 1000000 };
  Energy(float v, Unit u);
  float GetEnergy(Unit u);
  float GetEnergy();
  Unit GetUnit();
private:
  float value;
  Unit unit;
};

class Power {
public:
  Power(float v, Energy::Unit energyUnit, Time::Unit timeUnit);
  float GetPower() const;
  float GetPower(Energy::Unit e, Time::Unit t) const;
private:
  float value;
  Energy::Unit energyUnit;
  Time::Unit timeUnit;
};

#endif