#ifndef SCENE_DEF
#define SCENE_DEF

#include "standard.h"
#include "Entity.h"
#include "Camera.h"
#include "CollisionEngine.h"
#include "PhysicsEngine.h"
#include "Text.h"

class Entity;

class Scene {
public:
    Scene(float height, float width, float depth, glm::vec3 backgroundColor);
    ~Scene();

    virtual void Load();
    virtual void RestoreScene();
    // Unloads the current scene, BE CAREFUL WHEN CALLING, it will deallocate memory
    // Calling this function will clear GL program use
    virtual void Unload();
    // Use this function to safely unload the current scene while maintaining other
    // Scenes
    virtual void UnloadSafe(Scene *keep[], int n);
    
    // ABSTRACT FUNCTIONS
    virtual void Render() = 0;
    virtual void Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, const int mouseX, int mouseY) = 0;
    // Update the current scene, return address of next scene when advancing
    // Otherwise return NULL
    virtual Scene *Update(float delta) = 0;

    // Getters
    TexturedShader *GetTexturedRenderer() const;
    UntexturedShader *GetUntexturedRenderer() const;
    Scene *GetTransition(int index);

    // Adders
    void AddTransition(Scene *next);

    // Operators
    operator TexturedShader *() const;
    operator UntexturedShader *() const;
private:
    // Viewport
    float height, width, depth;

    glm::vec3 backgroundColor;

    glm::mat4 view;
    glm::mat4 projection;

    Camera camera;

    TexturedShader *rendererTextured;
    UntexturedShader *rendererUntextured;

    std::vector<Scene *> transitions;
};

class SimpleScene : public Scene {
public: 
    SimpleScene(float height, float width, float depth, glm::vec3 backgroundColor);
    virtual void Render();
    virtual void Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, const int mouseX, int mouseY);
    virtual Scene *Update(float delta);

    // Adders
    void AddEntity(Entity *entity);
private: 
    std::vector<Entity *> entities;
};

class MenuScene : virtual public Scene {
public:
    enum MenuSceneHorizontalAlignment { LEFT, CENTER, RIGHT };
    enum MenuSceneVerticalAlignment { TOP, MIDDLE, BOTTOM };
    
    MenuScene(std::vector<std::string> options, MenuSceneHorizontalAlignment h, MenuSceneVerticalAlignment v);
private:
    std::vector<Text *> labels;
};

class ActionScene : virtual public Scene {
public:
    ActionScene(float maxHeight, float maxWidth, float width, float height, float depth);
private:
    CollisionEngine collisions;
    PhysicsEngine physics;
};

class CompoundScene : virtual public Scene {
public:
    CompoundScene(float height, float width, float depth);
private:
    std::vector<Scene *> layers;
};


#endif