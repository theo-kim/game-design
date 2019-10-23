#include "include/QuadTree.h"

QuadTree::QuadTree() {
  rootNode = new QuadTreeNode(glm::vec3(0.0f), 0, 0);
  totalSize = 0;
}

QuadTree::QuadTree(glm::vec3 center, float height, float width) {
  rootNode = new QuadTreeNode(center, height, width);
  totalSize = 0;
}

QuadTree::~QuadTree() {
  delete rootNode;
}

int QuadTree::GetSize() {
  return totalSize;
}

QuadTree::QuadTreeNode::QuadTreeNode() {
  isLeaf = true;
  size = 0;
}

QuadTree::QuadTreeNode::QuadTreeNode(glm::vec3 c, float h, float w) {
  isLeaf = true;
  bucket = NULL;
  size = 0;
  totalSize = 0;
  topLeftChild = NULL;
  topRightChild = NULL;
  bottomLeftChild = NULL;
  bottomRightChild = NULL;
  center = c;
  left = c[0] - w / 2;
  right = c[0] + w / 2;
  top = c[1] + h / 2;
  bottom = c[1] - h / 2;
}

QuadTree::QuadTreeNode::QuadTreeNode(Entity *entity, glm::vec3 c, float h, float w) {
  isLeaf = true;
  bucket = new QuadTreeNode::EntityLinkedList(entity);
  size = 1;
  totalSize = 1;
  topLeftChild = NULL;
  topRightChild = NULL;
  bottomLeftChild = NULL;
  bottomRightChild = NULL;
  center = c;
  left = c[0] - w / 2;
  right = c[0] + w / 2;
  top = c[1] + h / 2;
  bottom = c[1] - h / 2;
}

QuadTree::QuadTreeNode::~QuadTreeNode() {
  if (!isLeaf) {
    delete topLeftChild;
    delete topRightChild;
    delete bottomLeftChild;
    delete bottomRightChild;
  }

  if (size > 0) {
    EntityLinkedList *csr = bucket;
    while (bucket != NULL) {
      bucket = bucket->Next();
      delete csr;
      csr = bucket;
    }
  }
}

Entity * QuadTree::QuadTreeNode::GetEntity (int index) const {
  EntityLinkedList *csr = bucket;
  int i = 0;
  while (csr != NULL && i < index) {
    if (i == index) {
      return csr->Get();
    }
    csr = csr->Next();
    ++i;
  }
  return NULL;
}

QuadTree::QuadTreeNode * QuadTree::QuadTreeNode::GetChild (bool top, bool left) const {
  if (isLeaf) return NULL;
  if (top) {
    if (left) {
      return topLeftChild;
    }
    else {
      return topRightChild;
    }
  }
  else {
    if (left) {
      return bottomLeftChild;
    }
    else {
      return bottomRightChild;
    }
  }
}

void QuadTree::QuadTreeNode::AddEntity(Entity *newEntity) {
  ++totalSize; // this will happen no matter what;
  // What to do if it IS a leaf
  if (isLeaf) {
    if (size + 1 > QUADTREE_THRESHOLD) {
      Decompose();
    }
    else {
      // Add to this node
      bucket = new EntityLinkedList(newEntity, bucket);
      ++size;
    }
  }
  // What to do if it is NOT a leaf
  if (!isLeaf) {
    // Check if its top half of the quadrant
    if (newEntity->CheckBounds(left, right, top, center[1])) {
      if (newEntity->CheckBounds(left, center[0], top, center[1])) {
	// It belongs to the TL quadrant
	topLeftChild->AddEntity(newEntity);
	return;
      }
      else if (newEntity->CheckBounds(center[0], right, top, center[1])) {
	// It belongs to the TR quadrant
	topRightChild->AddEntity(newEntity);
	return;
      }
    }
    // Check if its in the bottom half of the quadrant
    else if (newEntity->CheckBounds(left, right, center[1], bottom)) {
      if (newEntity->CheckBounds(left, center[0], center[1], bottom)) {
	// It belongs to the BL quadrant
	bottomLeftChild->AddEntity(newEntity);
	return;
      }
      else if (newEntity->CheckBounds(center[0], right, center[1], bottom)) {
	// It belongs to the BR quadrant
	bottomRightChild->AddEntity(newEntity);
	return;
      }
    }
    // If we've gotten here, it does not belong anywhere...
    // Add to this node
    bucket = new EntityLinkedList(newEntity, bucket);
    ++size;
  }
}

// THIS FUNCTION ONLY WORKS ON ENTITIES BELONGING TO THIS ENTITY
Entity * QuadTree::QuadTreeNode::PopEntity (int index) {
  Entity *output = NULL;
  EntityLinkedList *csr = bucket;
  int i = 0;
  while (csr != NULL && i < index) {
    if (i == index) {
      output = csr->Get();
      csr->Prev()->RelinkNext(csr->Next());
      csr->Next()->RelinkPrev(csr->Prev());
      delete csr;
      --size;
      --totalSize;
      return output;
    }
    csr = csr->Next();
    ++i;
  }
  return NULL;
}

// This function decomposes a quadrant into four new quads
void QuadTree::QuadTreeNode::Decompose () {
  if (!isLeaf) return;

  isLeaf = false;

  // First create subquadrants
  float halfHeight = std::fabs((top - bottom) / 2);
  float halfWidth = std::fabs((right - left) / 2);
  glm::vec3 adjustment = glm::vec3(halfWidth / 2, halfHeight / 2, 0.0f);

  glm::vec3 tl = center + adjustment * glm::vec3(-1.0f, 1.0f, 1.0f);
  glm::vec3 tr = center + adjustment * glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 bl = center + adjustment * glm::vec3(-1.0f, -1.0f, 1.0f);
  glm::vec3 br = center + adjustment * glm::vec3(1.0f, -1.0f, 1.0f);
  
  topLeftChild = new QuadTreeNode(tl, halfHeight, halfWidth);
  topRightChild = new QuadTreeNode(tr, halfHeight, halfWidth);
  bottomLeftChild = new QuadTreeNode(bl, halfHeight, halfWidth);
  bottomRightChild = new QuadTreeNode(br, halfHeight, halfWidth);

  // Reassign all entities
  size = 0;
  EntityLinkedList *l = bucket;
  EntityLinkedList *g = bucket;
  bucket = NULL;
  while (bucket != NULL) {
    AddEntity(l->Get());
    // Clean up the old list
    delete g;
    bucket = bucket->Next();
    g = bucket;
  }
}

int QuadTree::QuadTreeNode::Size () const {
  return totalSize;
}

int QuadTree::QuadTreeNode::SizeGetUnique () const {
  return size;
}

// THE MAIN REASON FOR THIS DAMN DS
std::vector<Entity *> QuadTree::QuadTreeNode::GetCollisions () const {
  std::vector<Entity *> output;
  if (totalSize < 2) {
    // No collisions are possible
    return output;
  }

  // TODO: Implement Sensor Checking...
  
  if (!isLeaf && size == 0) {
    // No overlaps, check the children...
    std::vector<Entity *> tlCollisions = topLeftChild->GetCollisions();
    std::vector<Entity *> trCollisions = topRightChild->GetCollisions();
    std::vector<Entity *> blCollisions = bottomLeftChild->GetCollisions();
    std::vector<Entity *> brCollisions = bottomRightChild->GetCollisions();

    output.insert(output.end(), tlCollisions.begin(), tlCollisions.end());
    output.insert(output.end(), trCollisions.begin(), trCollisions.end());
    output.insert(output.end(), blCollisions.begin(), blCollisions.end());
    output.insert(output.end(), brCollisions.begin(), brCollisions.end());
  }
  return output;
}

QuadTree::QuadTreeNode::EntityLinkedList::EntityLinkedList(Entity *e) {
  entity = e;
  after = NULL;
  before = NULL;
}

QuadTree::QuadTreeNode::EntityLinkedList::EntityLinkedList(Entity *e, EntityLinkedList *next) {
  entity = e;
  after = next;
  before = NULL;
}

Entity * QuadTree::QuadTreeNode::EntityLinkedList::Get() const {
  return entity;
}

QuadTree::QuadTreeNode::EntityLinkedList * QuadTree::QuadTreeNode::EntityLinkedList::Next() const {
  return after;
}

QuadTree::QuadTreeNode::EntityLinkedList * QuadTree::QuadTreeNode::EntityLinkedList::Prev() const {
  return before;
}

void QuadTree::QuadTreeNode::EntityLinkedList::RelinkNext(QuadTree::QuadTreeNode::EntityLinkedList *n) {
  after = n;
}

void QuadTree::QuadTreeNode::EntityLinkedList::RelinkPrev(QuadTree::QuadTreeNode::EntityLinkedList *p) {
  before = p;
}
