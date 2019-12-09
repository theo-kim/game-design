#include "physics/entity/PhysicsEntity.h"
#include "physics/forces/Gravity.h"

PhysicsEntity::PhysicsEntity(Position p, Orientation o, Shape s, Mass m, Mesh *mesh, ShaderProgram *shader) 
  : Entity(mesh, new Transformation(p, Transformation::Scale(), o), shader),
    mass(m),
    currentVelocity(glm::vec3(0), Length::Meter, Time::Seconds),
    currentAcceleration(glm::vec3(0), Length::Meter, Time::Seconds),
    currentPosition(p),
    currentOrientation(o),
    currentAngularVelocity(glm::vec3(0.0f), Time::Seconds),
    currentAngularAcceleration(glm::vec3(0.0f), Time::Seconds),
    shape(s),
    moment { Moment(s, Shape::X), Moment(s, Shape::Y), Moment(s, Shape::Z) }
{
}

void PhysicsEntity::ApplyForce(Force force) {
  // Increment acceleration by force
  currentAcceleration = currentAcceleration + (force / mass);
}

void PhysicsEntity::ApplyTorque(Torque torque) {
  currentAngularAcceleration = currentAngularAcceleration + (torque / moment);
}

void PhysicsEntity::Update(float delta) {
  Time deltaTime = Time(delta, Time::Seconds);

  // Linear space
  currentVelocity = currentVelocity + (currentAcceleration * deltaTime);
  Displacement displacement = currentVelocity * deltaTime; // Calculate displacement
  transformation->LocalTransform(displacement); // Change current position by displacement
  currentPosition = currentPosition + displacement;
  currentAcceleration = glm::vec3(0); // Zero out acceleration

  // Angular space
  currentAngularVelocity = currentAngularVelocity + (currentAngularAcceleration * deltaTime);
  AngularDisplacement aDisplacement = currentAngularVelocity * deltaTime;
  transformation->Transform(aDisplacement);
  currentOrientation = (Orientation)(currentOrientation + aDisplacement);
  currentAngularAcceleration = glm::vec3(0.0f);
}

// Gravity
void Gravity(PhysicsEntity& p1, PhysicsEntity& p2) {
  // Masses
  Large m1 = p1.mass.GetMass(Mass::Kilogram);
  Large m2 = p2.mass.GetMass(Mass::Kilogram);

  if (m1.exp + m2.exp < 10) return; // Ignore non-significant gravitational forces
  
  // Find displacement between the two bodies
  Displacement displacement = p1.currentPosition - p2.currentPosition;
  // Then the absolute distance between them
  float r = sqrt(displacement.scalar(Length::Meter));
  // Then the direction between them
  glm::vec3 direction = displacement.vector(Length::Meter);
  // Get force vector
  glm::vec3 fg = direction * (float)((G * m1 * m1) / pow(r, 3));
  
  // Apply forces
  p1.ApplyForce(Force(fg, Force::Newton));
  p2.ApplyForce(Force(fg * -1.0f, Force::Newton));
}