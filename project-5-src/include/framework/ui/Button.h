#ifndef BUTTON_UI_DEF
#define BUTTON_UI_DEF

#include "../gl.h"
#include "../Entity.h"
#include "../animation/standard.h"

class Button : public EntityGroup<Entity> {
public:
    enum ButtonState { CLICKED, HOVER, FOCUSSED, QUIET };
    
    Button(Entity *child);
    Button();

    virtual ButtonState Input(const SDL_Event &event, const Uint32 &mouse, float mouseX, float mouseY);
    virtual void Update(float delta);
    virtual void Render();

    // Getter
    ButtonState GetState() const;
private:
    bool InBounds(float x, float y);

    ButtonState currentState;
};

class AnimatedButton : public Button {
public:    
    AnimatedButton(Animation *hover, Animation *focus, Entity *entity);

    virtual void Update(float delta);
    virtual void Render();
private:
    int animState;
    int animState2;
    Animation *hover;
    Animation *focus;
};


#endif