#ifndef LANDER_DEF
#define LANDER_DEF

#include "standard.h"

class Lander {
 public:
  Lander();
  Lander(ShaderProgram *_program, glm::vec3 initPos, float initSize, const char *tex, int _f, float _m, float _t, float _r);
  void Render ();
  void Update (float delta);
  void ApplyForce (glm::vec3 f);
  void Thrust();
  void RotateClockwise();
  void RotateCounterclockwise();
  void StopRotation();
  glm::mat4x3 GetBounds ();
  float GetRotation();
  float GetMass();
  glm::vec3 GetForce(); // Get the force that the ship is currently exerting
  void Collision(bool good);
  void Crash();
  bool Alive();
  glm::vec3 GetPos();
  void ZoomTo(float scale);
  int GetFuel();
 private:
  void getTexVertices(float *coords);

  // Properties
  float mass; // Mass for acceleration adjustments
  int fuel;
  float thrust; // Force multiplier per unit of fuel
  float rotationalVelocity; // How fast the ship rotates
  
  // Rendering variables
  ShaderProgram *program;
  TextureSheet texture;
  int animState;
  float sizeRatio;

  // Transformation Variables
  glm::vec3 mov;
  glm::vec3 pos;
  glm::vec3 size;
  glm::vec3 zoom;
  glm::vec3 zoomShift;
  float rotation;
  float rotationDirection;

  // Physics
  glm::vec3 netForce;
  bool isThrusting;
  bool isExploded;
  bool isAlive;
  bool isLanded;
};

#endif
