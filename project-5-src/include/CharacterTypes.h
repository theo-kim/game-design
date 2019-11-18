#ifndef CHARACTERTYPE_DEF
#define CHARACTERTYPE_DEF

#include "standard.h"
#include "Character.h"
#include "framework/ui/Rect.h"
#include <SDL_mixer.h>

class Jim : public Character {
public:
    static TextureSheet *Sprite;
    Jim(TexturedShader *program, glm::vec3 pos);
    Jim(TexturedShader *program, glm::vec3 pos, float height);
    Jim(TexturedShader *program, PhysicsEngine *phys, CollisionEngine *col, glm::vec3 pos, float height);

    virtual void Ult(bool doit);
};

class Dwight : public Character {
public:
    static TextureSheet *Sprite;
    Dwight(TexturedShader *program, glm::vec3 pos);
    Dwight(TexturedShader *program, glm::vec3 pos, float height);
    Dwight(UntexturedShader *program, TexturedShader *_program, PhysicsEngine *phys, CollisionEngine *col, glm::vec3 pos, float height);

    virtual void Ult(bool doit);

    virtual void Update(float delta);
    virtual void Render();
    
    class Beet : virtual public Rect, public Physical, public Collidable {
    public:
        Beet(Dwight *owner, glm::vec3 vel, glm::vec3 origin);
        glm::vec3 velocity;

        virtual void Update(float delta);

        virtual void DidCollide(Collidable *with);
        virtual int CheckCollision(Collidable *with);
        virtual bool DidUpdate();
        virtual ColliderType GetColliderType();

        virtual glm::vec3 Push() const;
        virtual void PushBy(glm::vec3 force);
        virtual void GravitateTo(glm::vec3 direction, float g);

        std::vector<EdgeSensor> edges;
        Dwight *owner;
        bool kill;
        bool readytodie;
    };

    std::vector<Beet *> beets;
    UntexturedShader *beetProgram;
    float ultaccumulator;
};

class Michael : public Character {
public:
    static TextureSheet *Sprite;
    Michael(TexturedShader *program, glm::vec3 pos);
    Michael(TexturedShader *program, glm::vec3 pos, float height);
    Michael(TexturedShader *program, PhysicsEngine *phys, CollisionEngine *col, glm::vec3 pos, float height);

    virtual void Ult(bool doit);
    virtual void Update(float delta);

    float ultaccumulator;
};

class Stanley : public Character {
public:
    static TextureSheet *Sprite;
    Stanley(TexturedShader *program, glm::vec3 pos);
    Stanley(TexturedShader *program, glm::vec3 pos, float height);
    Stanley(TexturedShader *program, PhysicsEngine *phys, CollisionEngine *col, glm::vec3 pos, float height);

    virtual void Ult(bool doit);
    virtual void Update(float delta);

    float ultaccumulator;

    Mix_Chunk *effect;
};

class Jan : public Character {
public: 
    static TextureSheet *Sprite;

    Jan(TexturedShader *program, PhysicsEngine *phys, CollisionEngine *col, glm::vec3 pos, float height);

    virtual void Update(float delta);

    virtual void DidCollide(Collidable *with);
    virtual int CheckCollision(Collidable *with);

    float acc;
};

#endif