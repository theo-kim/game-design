#ifndef LENGTH_DEF
#define LENGTH_DEF

class Length {
public: 
  enum Unit { Game = 1, AU = 100000, Kilometer = 2, Meter = 1 };
  Length(float _length, Unit _unit) : value(_length), unit(_unit) {}
  float GetLength(Unit unit) const;
private:
  float value;
  Unit unit;
};

class Distance : public Length {
public:
  Distance(float _length, Unit _unit) : Length(_length, _unit) {}
};

#endif