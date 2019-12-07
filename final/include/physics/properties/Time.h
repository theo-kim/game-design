#ifndef TIME_DEF
#define TIME_DEF

#include "standard.h"

class Time {
public:
  enum Unit { Milliseconds = 1, Seconds = 1000, Minutes = 60000 };
  Time(float _value, Unit _unit);
  float GetTime(Unit unit) const;
  float GetTime() const;
  Unit GetUnit() const;
private:
  float value;
  Unit unit;
};

#endif