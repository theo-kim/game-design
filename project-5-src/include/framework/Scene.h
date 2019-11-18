#ifndef SCENE_DEF
#define SCENE_DEF

#include "gl.h"
#include "Entity.h"
#include "Camera.h"
#include "engines/standard.h"
#include "ui/Menu.h"

class Entity;

class Scene {
public:
    Scene(glm::vec3 size, glm::vec3 backgroundColor);
    Scene(glm::vec3 size, glm::vec4 backgroundColor);
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
    //virtual void AddEntity(Entity *newEntity) = 0;

    // Getters
    TexturedShader *GetTexturedRenderer() const;
    UntexturedShader *GetUntexturedRenderer() const;
    Scene *GetTransition(int index) const;
    int GetNumTransitions() const;
    glm::vec3 GetDimensions() const;
    Camera *GetCamera();

    // Adders
    void AddTransition(Scene *next);

    // Operators
    operator TexturedShader *() const;
    operator UntexturedShader *() const;
private:
    bool unloaded;
    // Viewport
    float height, width, depth;

    glm::vec4 backgroundColor;

    glm::mat4 view;
    glm::mat4 projection;

    Camera camera;

    TexturedShader *rendererTextured;
    UntexturedShader *rendererUntextured;

    std::vector<Scene *> transitions;
};

class SimpleScene : public Scene {
public: 
    SimpleScene(glm::vec3 dimensions, glm::vec3 background);
    virtual void Render();
    virtual void Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, const int mouseX, int mouseY);
    virtual Scene *Update(float delta);
    virtual void Unload();

    // Adders
    void AddEntity(Entity *entity);
    void AddEntity(std::initializer_list<Entity *> entities);
private: 
    std::vector<Entity *> entities;
};

class MenuScene : virtual public Scene {
public:
    MenuScene(glm::vec3 dimensions, glm::vec3 background);
    virtual void Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, const int mouseX, int mouseY);
    virtual Scene *Update(float delta);
    virtual void Render();
    virtual void Unload();

    // Getter
    int GetSelectedIndex() const;

    // Setters
    void SetMenu(Menu *entity);
private:
    Menu *menu;
    int optionChosen;
};

class ActionScene : virtual public Scene {
public:
    ActionScene(float maxHeight, float maxWidth, float width, float height, float depth);
private:
    CollisionEngine collisions;
    PhysicsEngine physics;
};

class CompoundScene : public Scene {
public:
    CompoundScene(glm::vec3 size, glm::vec3 col);

    virtual void Input(const SDL_Event &event, const Uint8 *keys, const Uint32 &mouse, const int mouseX, int mouseY);
    virtual Scene *Update(float delta);
    virtual void Render();
    virtual void Load();
    virtual void Unload();

    void AddLayer(glm::vec3 offset, Scene *newLayer);
private:
    std::vector<Scene *> layers;
};


#endif