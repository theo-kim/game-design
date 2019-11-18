#ifndef BUTTON_UI_DEF
#define BUTTON_UI_DEF

#include "../gl.h"
#include "../Entity.h"

class Button : public EntityGroup<Entity> {
public:
    enum ButtonState { CLICKED, HOVER, FOCUSSED, QUIET };
    
    Button(Entity *child);

    virtual ButtonState Input(const SDL_Event &event, const Uint32 &mouse, const int mouseX, int mouseY);
    virtual void Update(float delta);
    virtual void Render();

    // Getter
    ButtonState GetState() const;
private:
    bool InBounds(float x, float y);

    ButtonState currentState;
};

#endif