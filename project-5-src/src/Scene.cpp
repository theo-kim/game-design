#include "../include/Scene.h"

// 
// BEGIN SCENE DECLARATION :
Scene::Scene(float _height, float _width, float _depth, glm::vec3 _c)
    : height(_height),
      width(_width),
      depth(_depth),
      backgroundColor(_c),
      view(glm::mat4(1.0f)),
      camera(glm::vec3(0.0f), _height, _width, 1.0)
{}

Scene::~Scene() {}

void Scene::Load() {
    // Set up renderers
    rendererTextured = new ShaderProgram();
    rendererUntextured = new ShaderProgram();

    rendererTextured->Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    rendererUntextured->Load("shaders/vertex.glsl", "shaders/fragment.glsl");

    rendererTextured->SetProjectionMatrix(camera.GetProjection());
    rendererTextured->SetViewMatrix(view);

    rendererUntextured->SetProjectionMatrix(camera.GetProjection());
    rendererUntextured->SetViewMatrix(view);

    glUseProgram(rendererUntextured->programID);
    glUseProgram(rendererTextured->programID);

    // Set Background color
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
}

void Scene::RestoreScene() {
    glUseProgram(rendererUntextured->programID);
    glUseProgram(rendererTextured->programID);
}

void Scene::Unload() {
    glUseProgram(0);
    delete rendererTextured;
    delete rendererUntextured;
}

void Scene::UnloadSafe(Scene *keep[], int n) {
    Unload();
    for (int i = 0; i < n; ++i) {
        keep[i]->RestoreScene();
    }
}

ShaderProgram *Scene::GetTexturedRenderer() {
    return rendererTextured;
}
ShaderProgram *Scene::GetUntexturedRenderer() {
    return rendererUntextured;
}
Scene *Scene::GetTransition(int index) {
    return transitions[index];
}

void Scene::AddTransition(Scene *next) {
    transitions.push_back(next);
}
// :END SCENE DECLARATION
//
// 
// BEGIN SIMPLESCENE DECLARATION :
SimpleScene::SimpleScene(float height, float width, float depth, glm::vec3 backgroundColor) 
    : Scene(height, width, depth, backgroundColor) {}

void SimpleScene::Render() {
    for (int i = 0; i < entities.size(); ++i) {
        entities[i]->Render();
    }
}

void SimpleScene::Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, const int mouseX, int mouseY) {

}

Scene *SimpleScene::Update(float delta) {
    return NULL;
}
// : END SIMPLE SCENE DECLARATION
//
//
// BEGIN ACTION SCENE DECLARATION
ActionScene::ActionScene(float maxHeight, float maxWidth, float width, float height, float depth)
    : collisions(glm::vec3(0.0f, 0.0f, 1.0f), maxWidth, maxHeight) {}
// END ACTION SCENE DECLARATION
//

/*
for (int i = 0; i < foreground.size(); ++i) {
      foreground[i]->SetRenderFlag(foreground[i]->CheckRenderBounds(left, right, top, bottom));
    }
    */
