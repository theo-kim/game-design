#ifndef IMAGE_DEF
#define IMAGE_DEF

#include "../gl.h"
#include "../Entity.h"

class Image : public TexturedEntity {
public:
    Image(TexturedShader *program, const char *path, glm::vec3 pos, float width);

    // abstract implementations
    virtual void Render();
    virtual void Update(float delta);
    
private:
    std::string path;
};

#endif