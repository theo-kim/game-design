#ifndef QUADTREE_DEF
#define QUADTREE_DEF

#define QUADTREE_THRESHOLD 8

#include <map>
#include <iterator>
#include <utility>

#include "standard.h"
#include "Entity.h"

#define QUADTREE_DEAD_ENTITY 1
#define QUADTREE_NO_COLLISION 2
#define QUADTREE_YES_COLLISION 3
#define QUADTREE_ILLEGAL_COLLISION 4
#define QUADTREE_IGNORED_COLLISION 5
#define QUADTREE_ERROR_COLLISION 6

class QuadTree;
class Collidable;

// This is an ABSTRACT INTERFACE for Entities that can handle collisions
class Collidable : public Entity {
public:
  Collidable();
  Collidable(glm::vec3 _pos, glm::vec3 _size, float _rot, ShaderProgram *_program, QuadTree *engine);
  Collidable(glm::vec3 _pos, glm::vec3 _size, float _rot, TextureSheet *texture, ShaderProgram *_program, QuadTree *engine);
  ~Collidable();
  virtual void DidCollide(Collidable *with) = 0;
  virtual int CheckCollision(Collidable *with) = 0;
  virtual bool DidUpdate() = 0;
  virtual void Update(float delta);

  QuadTree *GetCollisionEngine();
  
  using Entity::CheckBounds;
  using Entity::GetPos;

  enum ColliderType { BALLISTIC, OBJECT };
  
  virtual ColliderType GetColliderType() = 0;
private:
  QuadTree *collisionEngine;
};

/// This is a Data Structure to handle finding collisions
class QuadTree {
public:
  QuadTree();
  QuadTree(glm::vec3 center, float height, float width);
  ~QuadTree(); // Deconstructor to deallocate Nodes

  void RegisterEntity(Collidable *entity);
  void CheckCollision();
  
  int GetSize();
  
  class QuadTreeNode {
  public:
    QuadTreeNode();
    QuadTreeNode(glm::vec3 center, float h, float w, QuadTreeNode *parent);
    QuadTreeNode(Collidable *entity, glm::vec3 center, float h, float w, QuadTreeNode *parent);
    ~QuadTreeNode(); // Deconstructor

    Collidable * GetEntity (int index) const;
    QuadTreeNode * GetChild (bool top, bool left) const;

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

    QuadTreeNode *parent;
    
    QuadTreeNode *topLeftChild;
    QuadTreeNode *topRightChild;
    QuadTreeNode *bottomLeftChild;
    QuadTreeNode *bottomRightChild;
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
    
    std::map<Collidable *, QuadTreeNode *> contains; // To keep track of which entities have been added
    EntityLinkedList *bucket;
  };

private:
  QuadTreeNode *rootNode;
  int totalSize;
};

#endif
