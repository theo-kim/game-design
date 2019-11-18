#ifndef RECT_UI_DEF
#define RECT_UI_DEF

#include "../Entity.h"
#include "../gl.h"

class Rect : public UntexturedEntity {
public:
    Rect(UntexturedShader *program, glm::vec3 pos, glm::vec3 size, float rot, glm::vec3 col);
    virtual void Render();
    virtual void Update(float delta);
};

#endif