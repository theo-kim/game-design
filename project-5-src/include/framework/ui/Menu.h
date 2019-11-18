#ifndef MENU_UI_DEF
#define MENU_UI_DEF

#include "../gl.h"
#include "../Entity.h"
#include "Button.h"
#include "Text.h"

class Menu : public EntityGroup<Button> {
public :
    Menu(TexturedShader *program, glm::vec3 pos, float textSize, float padding, std::initializer_list<const char *> labels);
    Menu(TexturedShader *program, glm::vec3 pos, float textSize, float padding, Text::Alignment align, std::initializer_list<const char *> labels);
    Menu(std::initializer_list<Button *> buttons);
    ~Menu();
    
    // Abstract Implementations
    virtual int Input(const SDL_Event &event, const Uint32 &mouse, const int mouseX, int mouseY);
    virtual void Update(float delta);
    virtual void Render();
};

#endif