#include "../include/framework/Scene.h"

// 
// BEGIN SCENE DECLARATION :
Scene::Scene(glm::vec3 size, glm::vec3 _c)
    : height(size[1]),
      width(size[0]),
      depth(size[2]),
      unloaded(true),
      backgroundColor(_c, 1.0f),
      view(glm::mat4(1.0f)),
      camera(glm::vec3(0.0f), size[1], size[0], 1.25)
{}

Scene::Scene(glm::vec3 size, glm::vec4 _c)
    : height(size[1]),
      width(size[0]),
      depth(size[2]),
      unloaded(true),
      backgroundColor(_c),
      view(glm::mat4(1.0f)),
      camera(glm::vec3(0.0f), size[1], size[0], 1.25)
{}

Scene::~Scene() {}

void Scene::Load() {
    // Set up renderers
    rendererTextured = new TexturedShader();
    rendererUntextured = new UntexturedShader();

    rendererTextured->Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    rendererUntextured->Load("shaders/vertex.glsl", "shaders/fragment.glsl");

    rendererTextured->SetProjectionMatrix(camera.GetProjection());
    rendererTextured->SetViewMatrix(view);

    rendererUntextured->SetProjectionMatrix(camera.GetProjection());
    rendererUntextured->SetViewMatrix(view);

    glUseProgram(rendererUntextured->programID);
    glUseProgram(rendererTextured->programID);

    // Set Background color
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);

    unloaded = false;
}

void Scene::RestoreScene() {
    glUseProgram(rendererUntextured->programID);
    glUseProgram(rendererTextured->programID);
}

void Scene::Unload() {
    if (unloaded) return;
    glUseProgram(0);
    delete rendererTextured;
    delete rendererUntextured;
    unloaded = true;
}

void Scene::UnloadSafe(Scene *keep[], int n) {
    if (unloaded) return;
    Unload();
    for (int i = 0; i < n; ++i) {
        keep[i]->RestoreScene();
    }
}

TexturedShader *Scene::GetTexturedRenderer() const {
    return rendererTextured;
}
UntexturedShader *Scene::GetUntexturedRenderer() const {
    return rendererUntextured;
}
Scene *Scene::GetTransition(int index) const {
    return transitions[index];
}
int Scene::GetNumTransitions() const {
    return transitions.size();
}
glm::vec3 Scene::GetDimensions() const {
    return glm::vec3(width, height, depth);
}
Camera *Scene::GetCamera() {
    return &camera;
}

void Scene::AddTransition(Scene *next) {
    transitions.push_back(next);
}

Scene::operator TexturedShader *() const {
    return GetTexturedRenderer();
}

Scene::operator UntexturedShader *() const {
    return GetUntexturedRenderer();
}
// :END SCENE DECLARATION
//
// 
// BEGIN SIMPLESCENE DECLARATION :
SimpleScene::SimpleScene(glm::vec3 dimensions, glm::vec3 background) 
    : Scene(dimensions, background) 
{}

void SimpleScene::Render() {
    for (int i = 0; i < entities.size(); ++i) {
        entities[i]->Render();
    }
}

void SimpleScene::Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, const int mouseX, int mouseY) {}

Scene *SimpleScene::Update(float delta) {
    for (int i = 0; i < entities.size(); ++i) {
        entities[i]->Update(delta);
    }
    return NULL;
}

void SimpleScene::Unload() {
    for (int i = 0; i < entities.size(); ++i) {
        delete entities[i];
    }
    Scene::Unload();
}

void SimpleScene::AddEntity(Entity *e) {
    entities.push_back(e);
}

void SimpleScene::AddEntity(std::initializer_list<Entity *> _entities) {
    for (Entity *x : _entities) {
        entities.push_back(x);
    }
}
// : END SIMPLE SCENE DECLARATION
//
//
// BEGIN ACTION SCENE DECLARATION :
ActionScene::ActionScene(float maxHeight, float maxWidth, float width, float height, float depth)
    : collisions(glm::vec3(0.0f, 0.0f, 1.0f), maxWidth, maxHeight) {}
// END ACTION SCENE DECLARATION
//
//
// BEGIN MENU SCENE DECLARATION :
MenuScene::MenuScene(glm::vec3 dimensions, glm::vec3 background)
    : Scene(dimensions, background) {}

void MenuScene::Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, const int mouseX, int mouseY) {
    optionChosen = menu->Input(event, mouse, mouseX, mouseY);
}

Scene *MenuScene::Update(float delta) {
    menu->Update(delta);
    return NULL;
}

void MenuScene::Render() {
    menu->Render();
}

void MenuScene::Unload() {
    delete menu;
    Scene::Unload();
}

// Getters
int MenuScene::GetSelectedIndex() const {
    return optionChosen;
}

// Setters
void MenuScene::SetMenu(Menu *entity) {
    menu = entity;
}
// : END MENU SCENE DECLARATION
//

CompoundScene::CompoundScene(glm::vec3 size, glm::vec3 col)
    : Scene(size, col) {}

void CompoundScene::Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, const int mouseX, int mouseY) {
    for (Scene *layer : layers) {
        layer->Input(event, keys, mouse, mouseX, mouseY);
    }
}

Scene *CompoundScene::Update(float delta) {
    for (Scene *layer : layers) {
        Scene *out = layer->Update(delta);
        if (out != NULL) return out;
    }
    return NULL;
}

void CompoundScene::Render() {
    for (Scene *layer : layers) {
        layer->Render();
    }
}

void CompoundScene::Load() {
    for (Scene *layer : layers) {
        layer->Load();
    }
}

void CompoundScene::Unload() {
    for (Scene *layer : layers) {
        layer->Unload();
    }
}

void CompoundScene::AddLayer(glm::vec3 offset, Scene *newLayer) {
    layers.push_back(newLayer);
    newLayer->GetCamera()->PanBy(offset);
}


/*
for (int i = 0; i < foreground.size(); ++i) {
      foreground[i]->SetRenderFlag(foreground[i]->CheckRenderBounds(left, right, top, bottom));
    }
    */
