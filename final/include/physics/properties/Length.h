#ifndef LENGTH_DEF
#define LENGTH_DEF

#include "standard.h"

class Length {
public: 
  enum Unit : unsigned long { Game = 1, AU = 140000000000, Kilometer = 1000, Meter = 1 };
  Length(float _length, Unit _unit) : value(_length), unit(_unit) {}
  float GetLength(Unit unit) const;
  float GetLength() const;
  Unit GetUnit() const;
private:
  float value;
  Unit unit;
};

class Distance : public Length {
public:
  Distance(float _length, Unit _unit) : Length(_length, _unit) {}
};

#endif