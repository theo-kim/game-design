#include "../include/framework/ui/standard.h"
//
// BEGIN IMAGE DECLARATION :
Image::Image(TexturedShader *program, const char *path, glm::vec3 pos, float width)
    : TexturedEntity(program, new TextureSheet(path, 1, 1)),
      Entity(program, pos, glm::vec3(1.0f), 0.0f)
{
    SetSize(glm::vec3(width, (width / GetTexture()->GetSize()), 1.0f));
}

// abstract implementations
void Image::Render() {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, GetPos());
    modelMatrix = glm::rotate(modelMatrix, GetRot(), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, GetSize()); 

    float map[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5,
            -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1 };

    GetTexture()->GetSprite(map + 12, 1, 1);

    TexturedEntity::Render(modelMatrix, map, 12);
}

void Image::Update(float delta) {
    return;
}
// END IMAGE DECLARATION
//
// 
// BEGIN RECT DECLARATION :
Rect::Rect(UntexturedShader *program, glm::vec3 pos, glm::vec3 size, float rot, glm::vec3 col)
    : UntexturedEntity(program, col),
      Entity(program, pos, size, rot) {}

void Rect::Render() {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, GetPos());
    modelMatrix = glm::rotate(modelMatrix, GetRot(), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, GetSize()); 
    
    float map[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5,
            -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1 };
    
    UntexturedEntity::Render(modelMatrix, map, 12);
}

void Rect::Update(float delta) { return; }
// : END RECT DECLARATION
//
// 
// BEGIN BUTTON DECLARATION :
Button::Button(Entity *child)
    : EntityGroup<Entity>({child})
{
    SetSize(GetEntity(0)->GetSize());
    SetRot(GetEntity(0)->GetRot());
    SetPos(GetEntity(0)->GetPos());
}

Button::ButtonState Button::Input(const SDL_Event &event, const Uint32 &mouse, const int mouseX, int mouseY) {
    if (event.type == SDL_MOUSEBUTTONDOWN && (GetState() == HOVER || GetState() == FOCUSSED)) {
        std::cout << "FOCUSSED" << std::endl;      
        currentState = FOCUSSED;
    }
    else if (event.type == SDL_MOUSEBUTTONUP && GetState() == FOCUSSED && InBounds(mouseX, mouseY)) {
        std::cout << "CLICKED" << std::endl;
        currentState = CLICKED;
    }
    else if (InBounds(mouseX, mouseY)) {
        std::cout << "HOVER" << std::endl;
        currentState = HOVER;
    }
    else {
        currentState = QUIET;
    }
    return currentState;
}

void Button::Update(float delta) {
    return;
}

void Button::Render() {
    GetEntity(0)->Render();
}

// Getter
Button::ButtonState Button::GetState() const {
    return currentState;
}

// Private
bool Button::InBounds(float x, float y) {
    float wOffset = GetSize()[0] / 2;
    float hOffset = GetSize()[1] / 2;
    float left = GetPos()[0] - wOffset;
    float right = GetPos()[0] + wOffset;
    float top = GetPos()[1] + hOffset;
    float bottom = GetPos()[1] - hOffset;

    if (x > right || x < left || y > top || y < bottom ) {
        return false;
    }
    return true;
}
// : END BUTTON DECLARATION
//
//
// START MENU DECLARATION :
Menu::Menu(TexturedShader *program, glm::vec3 pos, float textSize, float padding, std::initializer_list<const char *> labels) 
    : EntityGroup<Button>()
{ 
    int numLabels = labels.size();
    float paddedSize = textSize + padding * 2;
    int i = 0;
    for (const char *label : labels) {
        float y = pos[1] + (numLabels * paddedSize / 2) - i * paddedSize;
        glm::vec3 optionPos(pos[0], y, pos[2]);
        EntityGroup<Button>::AddEntity(new Button(
            new Text(label, program, optionPos, textSize)
        ));
        ++i;
    }
}
Menu::Menu(TexturedShader *program, glm::vec3 pos, float textSize, float padding, Text::Alignment align, std::initializer_list<const char *> labels) 
    : EntityGroup<Button>()
{ 
    int numLabels = labels.size();
    float paddedSize = textSize + padding * 2;
    int i = 0;
    for (const char *label : labels) {
        float y = pos[1] + (numLabels * paddedSize / 2) - i * paddedSize;
        glm::vec3 optionPos(pos[0], y, pos[2]);
        EntityGroup<Button>::AddEntity(new Button(
            new Text(label, program, optionPos, textSize, align)
        ));
        ++i;
    }
}
Menu::Menu(std::initializer_list<Button *> buttons)
    : EntityGroup<Button>(buttons)
{}

Menu::~Menu() {}

// Abstract Implementations
int Menu::Input(const SDL_Event &event, const Uint32 &mouse, const int mouseX, int mouseY) {
    for (int i = 0; i < GetNumChildren(); ++i) {
        if (GetEntity(i)->Input(event, mouse, mouseX, mouseY) == Button::CLICKED) {
            return i;
        }
    }
    return -1;
}
void Menu::Update(float delta) {
    for (int i = 0; i < GetNumChildren(); ++i) {
        GetEntity(i)->Update(delta);
    }
}
void Menu::Render() {
    for (int i = 0; i < GetNumChildren(); ++i) {
        GetEntity(i)->Render();
    }
}
// : END MENU DECLARATION
//