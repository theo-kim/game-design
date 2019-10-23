#ifndef QUADTREE_DEF
#define QUADTREE_DEF

#define QUADTREE_THRESHOLD 8

#include "standard.h"
#include "Entity.h"

class QuadTree {
public:
  QuadTree();
  QuadTree(glm::vec3 center, float height, float width);
  ~QuadTree(); // Deconstructor to deallocate Nodes

  int GetSize();
  
  class QuadTreeNode {
  public:
    QuadTreeNode();
    QuadTreeNode(glm::vec3 center, float h, float w);
    QuadTreeNode(Entity *entity, glm::vec3 center, float h, float w);
    ~QuadTreeNode(); // Deconstructor

    Entity * GetEntity (int index) const;
    QuadTreeNode * GetChild (bool top, bool left) const;

    void AddEntity(Entity *newEntity);
    Entity *PopEntity(int index); // Remove entity and return;
    void Decompose(); // (only if a leaf) decompose into four children
    
    int Size () const; // Get Number of entities belonging to this AND children
    int SizeGetUnique () const; // Get Number of entity ONLY belonging to this node

    std::vector<Entity *> GetCollisions () const;
    
    bool isLeaf;
  private:
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
      EntityLinkedList(Entity *e);
      EntityLinkedList(Entity *e, EntityLinkedList *next);
      EntityLinkedList * Next() const;
      EntityLinkedList * Prev() const;
      Entity * Get() const;
      void RelinkNext(EntityLinkedList *n);
      void RelinkPrev(EntityLinkedList *p);
    private:
      Entity *entity;
      EntityLinkedList *before;
      EntityLinkedList *after;
    };
    
    EntityLinkedList *bucket;
  };

private:
  QuadTreeNode *rootNode;
  int totalSize;
};

#endif
