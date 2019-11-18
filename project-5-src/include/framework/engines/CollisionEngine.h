#ifndef QUADTREE_DEF
#define QUADTREE_DEF

#define QUADTREE_THRESHOLD 8

#include <map>
#include <iterator>
#include <utility>

#include "../gl.h"
#include "../Entity.h"

#define QUADTREE_DEAD_ENTITY 1
#define QUADTREE_NO_COLLISION 2
#define QUADTREE_YES_COLLISION 3
#define QUADTREE_ILLEGAL_COLLISION 4
#define QUADTREE_IGNORED_COLLISION 5
#define QUADTREE_ERROR_COLLISION 6

class CollisionEngine;
class Collidable;

// This is an ABSTRACT INTERFACE for Entities that can handle collisions
class Collidable : virtual public Entity {
public:
  enum ColliderType { BALLISTIC, CHARACTER, SURFACE };

  Collidable(CollisionEngine *engine);
  ~Collidable();
  virtual void DidCollide(Collidable *with) = 0;
  virtual int CheckCollision(Collidable *with) = 0;
  virtual bool DidUpdate() = 0;
  virtual ColliderType GetColliderType() = 0;
  virtual void Update(float delta);

  CollisionEngine *GetCollisionEngine();
  
  using Entity::CheckBounds;
  using Entity::GetPos;

private:
  CollisionEngine *collisionEngine;
};

/// This is a Data Structure to handle finding collisions
class CollisionEngine {
public:
  CollisionEngine();
  CollisionEngine(glm::vec3 center, float height, float width);
  ~CollisionEngine(); // Deconstructor to deallocate Nodes

  void RegisterEntity(Collidable *entity);
  void CheckCollision();
  
  int GetSize();
  
  class CollisionEngineNode {
  public:
    CollisionEngineNode();
    CollisionEngineNode(glm::vec3 center, float h, float w, CollisionEngineNode *parent);
    CollisionEngineNode(Collidable *entity, glm::vec3 center, float h, float w, CollisionEngineNode *parent);
    ~CollisionEngineNode(); // Deconstructor

    Collidable * GetEntity (int index) const;
    int GetIndex (Collidable *target) const;
    CollisionEngineNode * GetChild (bool top, bool left) const;
    bool InScope(Collidable *target) const;

    void AddEntity(Collidable *newEntity);
    Collidable *PopEntity(int index); // Remove entity and return
    Collidable *PopEntity(Collidable *target); // Remove entity by pointer
    void Decompose(); // (only if a leaf) decompose into four children

    //void PrintQuadrant(Collidable *target);
    
    int Size () const; // Get Number of entities belonging to this AND children
    int SizeGetUnique () const; // Get Number of entity ONLY belonging to this node

    void GetCollisions ();
    void GetCollisions (Collidable *with, int thisBorder, int borderFlags);

    bool Contains(Collidable *object);
    
    bool isLeaf;
  private:
    int GetBorderedQuadrants(Collidable *object);

    CollisionEngineNode *parent;
    
    CollisionEngineNode *topLeftChild;
    CollisionEngineNode *topRightChild;
    CollisionEngineNode *bottomLeftChild;
    CollisionEngineNode *bottomRightChild;
    int size;
    int totalSize;

    glm::vec3 center; // Center coordinate of the quadrant
    float top;
    float bottom;
    float left;
    float right;
   
    class EntityLinkedList {
    public:
      EntityLinkedList(Collidable *e, int borderFlags);
      EntityLinkedList(Collidable *e, int borderFlags, EntityLinkedList *next);
      EntityLinkedList * Next() const;
      EntityLinkedList * Prev() const;
      Collidable * Get() const;
      int GetBorderFlags() const;
      void RelinkNext(EntityLinkedList *n);
      void RelinkPrev(EntityLinkedList *p);
    private:
      Collidable *entity;
      int borderFlags;
      EntityLinkedList *before;
      EntityLinkedList *after;
    };
    
    std::map<Collidable *, CollisionEngineNode *> contains; // To keep track of which entities have been added
    EntityLinkedList *bucket;
  };

private:
  CollisionEngineNode *rootNode;
  int totalSize;
};

#endif
