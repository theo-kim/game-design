#ifndef MASS_DEF
#define MASS_DEF

#include "Acceleration.h"

class Mass {
public: 
  class Unit {
  public:
    Unit(Large l) : value(l) {}
    Large value;
    bool operator==(const Unit& u) const { return value == u.value; }
  };
  static Unit Gram;
  static Unit Kilogram;
  static Unit Megagram;
  static Unit Earth;
  static Unit Solar;
  
  Mass(Large _mass, Unit _unit) : value(_mass), unit(_unit) {}
  Large GetMass(Unit unit) const;
  // Force operator*(const Acceleration& a) const;
private:
  Large value;
  Unit unit;
};

#endif