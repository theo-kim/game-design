#include "game/scenes/GameScene.h"

float randomAngle() {
  return radians((random() % (200 + 200) - 200) / 10.0f);
}

Position randomPosition(glm::vec3 point) {
  float x = (random() % (30 + 30) - 30) / 10.0f;
  float y = (random() % (30 + 30) - 30) / 10.0f;
  float z = 0.0f;//(random() % (10 + 10) - 10) / 10.0f;

  return Position(point + glm::vec3(x, y, z), Length::Kilometer);
}

Velocity randomDirection() {
  float x = (random() % (100 + 100) - 100) / 10.0f;
  float y = (random() % (100 + 100) - 100) / 10.0f;
  float z = (random() % (100 + 100) - 100) / 10.0f;

  return Velocity(glm::vec3(x, y, z), Length::Meter, Time::Seconds);
}

std::vector<glm::vec3> GeneratePath(int length) {
  std::vector<glm::vec3> path;
  glm::vec3 currentPoint = glm::vec3(0.0f);
  for (int i = 0; i < length; ++i) {
    path.push_back(currentPoint);
    glm::vec4 magnitude = glm::vec4(0.0f, 0.0f, (rand() % (40 + 10) / 10.0f), 0.0f);
    glm::quat direction = glm::quat(glm::vec3(radians(rand() % ((20 + 20) - 20)), radians(rand() % ((20 + 20) - 20)), 0.0f));
    currentPoint = currentPoint - glm::vec3(magnitude * direction);
  }
  return path;
}

GameScene::GameScene(SDL_Window* _window, float width, float height) 
  : Scene(_window),
    ready(false) {
  // Initialize shaders
  ShaderProgram *modelShader = new ShaderProgram();
  ShaderProgram *sunShader = new ShaderProgram();
  
  // Initialize entities
  sun = new Sun(Position(glm::vec3(-0.25f, 0.0f, 0.0f), Length::AU), Mass(Large(1.0f, 0.0f), Mass::Solar), 
    Length(695510.0f, Length::Kilometer), Power(10e16, Energy::Kilojoule, Time::Seconds),
    Mesh3D::FromOBJ("models/sphere-test.obj"), sunShader, glm::vec3(1.0f));
  
  planet = new Planet(Position(glm::vec3(0.0f, 0.0f, -0.0002f), Length::AU),
    Mass(Large(1.0f, 0.0f), Mass::Earth), Length(6371.0f, Length::Kilometer), 
    Mesh3D::FromOBJ("models/sphere-smooth-test.obj"), modelShader);

  // Initialize camera
  glm::mat4 perspective = glm::perspective(radians(45.0f), width / height, 1.0f, 100000.0f);
  camera = new Camera(
    new Transformation(),
    perspective
  );

  // Set up shaders
  sunShader->Load("shaders/vertex.glsl", "shaders/fragment.glsl");
  sunShader->SetCamera(camera);
  sunShader->SetColor(0.8f, 0.8f, 0.6f, 1.0f);

  ShipFuel::shipFuelShader = modelShader;

  modelShader->Load("shaders/vertex_diffuse.glsl", "shaders/fragment_diffuse.glsl");
  modelShader->SetCamera(camera);
  modelShader->SetLight(&sun->GetLight());
  modelShader->EnableLighting();
  modelShader->SetColor(0.0f, 0.4f, 0.9f, 1.0f);

  ships.push_back(new Ship(
    Position(glm::vec3(0.0f, 0.0f, 0.0f), Length::Kilometer),
    Orientation(glm::vec3(0.0f, 0.0f, 0.0f)),
    new Cylinder(
      Mass(Large(1000.0f, 0.0f), Mass::Kilogram),
      Length(20.0f, Length::Meter),
      Length(35.0f, Length::Meter) 
    ),
    ShipConfiguration(
      Hull(10, Mass(100.0f, Mass::Kilogram)),
      Engine(Force(glm::vec3(0, 0, -75.0f), Force::Kilonewton), Mass(10.0f, Mass::Kilogram)),
      Battery(Energy(10.0f, Energy::Joule), Mass(10.0f, Mass::Kilogram)),
      Reactor(Power(10.0f, Energy::Joule, Time::Seconds), Mass(10.0f, Mass::Kilogram)),
      Shield(Energy(100.0f, Energy::Joule)),
      Thruster(Torque(glm::vec3(0.0f, radians(0.5f), 0.0f), Force::Kilonewton, Length::Meter), Mass(1.0f, Mass::Kilogram)),
      Thruster(Torque(glm::vec3(radians(0.5f), 0.0f, 0.0f), Force::Kilonewton, Length::Meter), Mass(1.0f, Mass::Kilogram)),
      Cockpit(Transformation(Translation(glm::vec3(0.0f, 0.0f, 300.0f)), Scale(), Rotation()), 
              glm::mat3(glm::vec3(radians(-75.0f), radians(75.0f), 5.0f),
                        glm::vec3(radians(-75.0f), radians(75.0f), 5.0f),
                        glm::vec3()))
    ),
    Mesh3D::FromOBJ("models/ship.obj"),
    modelShader,
    glm::vec3(0.85f, 0.85f, 0.85f)
  ));

  // Render asteroids
  int numberOfRoids = 50;
  std::vector<glm::vec3> path = GeneratePath(numberOfRoids);
  for (int i = 0; i < numberOfRoids - 1; ++i) {
    int r = rand() % 4;
    for (int j = 0; j < r; ++j) {
      asteroids.push_back(new Asteroid(
        randomPosition(path[i]),
        Mass(500.0f, Mass::Kilogram),
        Length(random() % (200 - 50) + 50, Length::Meter),
        AngularVelocity(glm::vec3(randomAngle(), randomAngle(), randomAngle()), Time::Seconds),
        randomDirection(),
        Mesh3D::FromOBJ("models/asteroid.obj"),
        modelShader
      ));
    }
  }

  station = new Station(Position(path[numberOfRoids - 1], Length::Kilometer),
    Mass(1.0f, Mass::Kilogram), Length(200.0f, Length::Meter), 
    Mesh3D::FromOBJ("models/station.obj"), modelShader);

  // Set up controllers
  controllers.push_back(new Human(ships[0]));
  controllers.push_back(new CollisionController(ships[0], this));

  // Set up camera tracking
  camera->BindToEntity(*ships[0], Transformation());

  SDL_SetRelativeMouseMode(SDL_TRUE);

  camera->ConstrainRotation(Camera::X, radians(-30.0f), radians(30.0f));
  camera->ConstrainRotation(Camera::Y, radians(-30.0f), radians(30.0f));

  // Load and start music
  music = Mix_LoadMUS( "sounds/music.wav" );
  Mix_VolumeMusic(MIX_MAX_VOLUME * 0.5);
  Mix_PlayMusic( music, -1 );
  Mix_Volume(-1, MIX_MAX_VOLUME * 0.5);

  // Load fonts
  ShaderProgram *textShader = new ShaderProgram();
  textShader->Load("shaders/vertex_textured_2D.glsl", "shaders/fragment_textured_2D.glsl");
  textShader->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
  // gFont = TTF_OpenFont("textures/fonts/Muli/static/Muli-Regular.ttf", 28);
  cameraOverlay = new Camera(new Transformation(Translation(0.0f, 0.0f, 0.0f), Scale(1.0f), Rotation()), 
    glm::ortho(-5.0f, 5.0f, -(height / width) * 5.0f, (height / width) * 5.0f, -1.0f, 1.0f));
  textShader->SetCamera(cameraOverlay);
  test = new Text("textures/fonts/Muli/static/Muli-Regular.ttf", 42, glm::vec3(1.0f),
    new Transformation(Translation(glm::vec3(0.0f, 0.0f, 0.0f)), Scale(2.0f, 0.3f, 1.0f), Rotation(0.0f)), textShader);
  test->SetText("Welcome to the Gauntlet!");

  welcome = new Text("textures/fonts/Muli/static/Muli-Regular.ttf", 36, glm::vec3(1.0f),
    new Transformation(Translation(glm::vec3(0.0f, -0.5f, 0.0f)), Scale(2.0f, 0.2f, 1.0f), Rotation(0.0f)), textShader);
  welcome->SetText("Press Enter to Continue");
}

GameScene::~GameScene() {
  Mix_HaltMusic();
  Mix_FreeMusic( music );
}

void GameScene::Render() {
  if (!ready) {
    welcome->Render();
    test->Render();
    return;
  }
  planet->Render();
  sun->Render();
  station->Render();
  for (Ship *s : ships) {
    s->Render();
    Gravity(*s, *planet);
  }
  for (Asteroid *a : asteroids) {
    // s->transformation->Transform(Translation(0.0f, 0.0f, 10000.0f));
    a->Render();
    // Gravity(*s, *planet);
  }
  for (Entity *e : overlay) {
    e->Render();
  }
  if (ships[0]->dead) {
    test->Render();
    welcome->Render();
    // ship->dead = true;
  }
  if (controllers[1]->GetState().currentState == 1) {
    test->SetText("You win!");
    test->Render();
  }
}

float para(float t)
{
    static float last;
    float full = pow(t, 2) * (3.0f - 2.0f * t);
    float out = full - last;
    last = full;
    if (last == 1) last = 0;
    if (out < 0) return 0;
    return out;
}

void GameScene::Update(float delta) {
  static float accumulator;
  if (!ready) {
    if (accumulator > 2.0f) {
      test->transformation->Transform(Translation(glm::vec3(0.0f, -0.25f, 0.0f) * para((accumulator - 2.0f) / 2.0f)));
      accumulator += delta;
    }
    else {
      test->transformation->Transform(Translation(glm::vec3(0.0f, 0.25f, 0.0f) * para(accumulator / 2.0f)));
      accumulator += delta;
    }
    if (accumulator > 4.0f) {
      accumulator = 0;
    }
    return;
  }
  for (Ship *s : ships) {
    s->Update(delta);
    Gravity(*s, *planet);
  }
  for (Asteroid *a : asteroids) {
    a->Update(delta);
    // Gravity(*s, *planet);
  }
  for (Controller *c : controllers) {
    c->Update(delta);
  }
  return;
}

void GameScene::Input(SDL_Event& e) {
  if (!ready) {
    if (e.type == SDL_KEYDOWN) {
      if (e.key.keysym.sym == SDLK_RETURN) {
        ready = true;
        test->SetText("You lost...");
        welcome->SetText("Press ESCAPE to restart.");
      }
    }
    return;
  }
  for (Controller *c : controllers) {
    c->Input(e);
  }
  return;
}

// Opening Scene
