#include "game/scenes/GameScene.h"

GameScene::GameScene(float width, float height) {
  // Initialize shaders
  ShaderProgram *modelShader = new ShaderProgram();
  ShaderProgram *sunShader = new ShaderProgram();
  
  // Initialize entities
  sun = new Sun(Position(glm::vec3(0.0f), Length::AU), Mass(Large(1.0f, 0.0f), Mass::Solar), 
    Length(695510.0f, Length::Kilometer), Power(10e1, Energy::Kilojoule, Time::Seconds),
    Mesh3D::FromOBJ("models/sphere-test.obj"), sunShader, glm::vec3(1.0f));
  
  planet = new Planet(Position(glm::vec3(0.0f, 0.0f, 1.0f), Length::AU),
    Mass(Large(1.0f, 0.0f), Mass::Earth), Length(6371.0f, Length::Kilometer), 
    Mesh3D::FromOBJ("models/sphere-smooth-test.obj"), modelShader);

  // Initialize camera
  glm::mat4 perspective = glm::perspective(radians(45.0f), width / height, 0.1f, 100.0f);
  camera = new Camera(
    new Transformation(
      Position(glm::vec3(0.0f, 0.0f, 100.0f), Length::Game), 
      Transformation::Scale(), 
      Transformation::Rotation(0.0f, radians(0.0f), 0.0f)
    ),
    perspective
  );

  // Set up shaders
  modelShader->Load("shaders/vertex_diffuse.glsl", "shaders/fragment_diffuse.glsl");
  modelShader->SetCamera(camera);
  modelShader->SetLight(&sun->GetLight());
  modelShader->EnableLighting();
  modelShader->SetColor(0.0f, 0.4f, 0.9f, 1.0f);

  sunShader->Load("shaders/vertex.glsl", "shaders/fragment.glsl");
  sunShader->SetCamera(camera);
  sunShader->SetColor(0.8f, 0.8f, 0.6f, 1.0f);

  ships.push_back(new Ship(
    Position(glm::vec3(0.0f, 0.0f, 100000.0f), Length::Kilometer),
    Orientation(glm::vec3(0.0f)),
    Cylinder(
      Mass(Large(1000.0f, 0.0f), Mass::Kilogram),
      Length(20.0f, Length::Meter),
      Length(35.0f, Length::Meter) 
    ),
    ShipConfiguration(
      Hull(10, Mass(100.0f, Mass::Kilogram)),
      Engine(Force(glm::vec3(0, 0, 100.0f), Force::Kilonewton), Mass(10.0f, Mass::Kilogram)),
      Battery(Energy(10.0f, Energy::Joule), Mass(10.0f, Mass::Kilogram)),
      Reactor(Power(10.0f, Energy::Joule, Time::Seconds), Mass(10.0f, Mass::Kilogram)),
      Shield(Energy(100.0f, Energy::Joule)),
      Thruster(Torque(glm::vec3(0.0f, 0.0f, 100.0f), Force::Newton, Length::Meter), Mass(1.0f, Mass::Kilogram)),
      Thruster(Torque(glm::vec3(100.0f, 0.0f, 0.0f), Force::Newton, Length::Meter), Mass(1.0f, Mass::Kilogram))
    ),
    Mesh3D::FromOBJ("models/cube-test.obj"),
    modelShader
  ));

  camera->BindToEntity(*ships[0], Transformation(Displacement(glm::vec3(0.0f, 0.0f, 75.0f), Length::Meter), Scale(), Rotation()));
}

GameScene::~GameScene() {

}

void GameScene::Render() {
  for (Ship *s : ships) {
    s->Render();
  }
  // planet->Render();
  // sun->Render();
}

void GameScene::Update(float delta) {
  return;
}

void GameScene::Input() {
  return;
}