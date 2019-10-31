#include "include/QuadTree.h"

Collidable::Collidable() {}

Collidable::Collidable(glm::vec3 _pos, glm::vec3 _size, float _rot, ShaderProgram *program, QuadTree *engine)
  : Entity(_pos, _size, _rot, program),
    collisionEngine(engine)
{}

Collidable::Collidable(glm::vec3 _pos, glm::vec3 _size, float _rot, TextureSheet *tex, ShaderProgram *program, QuadTree *engine)
  : Entity(_pos, _size, _rot, tex, program),
    collisionEngine(engine)
{}

Collidable::~Collidable() {}

QuadTree *Collidable::GetCollisionEngine() {
  return collisionEngine;
}

void Collidable::Update(float delta) {
  if (collisionEngine != NULL) {
    collisionEngine->RegisterEntity(this);
  }
}

QuadTree::QuadTree() {
  rootNode = new QuadTreeNode(glm::vec3(0.0f), 0, 0, NULL);
  totalSize = 0;
}

QuadTree::QuadTree(glm::vec3 center, float height, float width) {
  rootNode = new QuadTreeNode(center, height, width, NULL);
  totalSize = 0;
}

QuadTree::~QuadTree() {
  delete rootNode;
}

void QuadTree::RegisterEntity(Collidable *entity) {
  rootNode->AddEntity(entity);
}

void QuadTree::CheckCollision() {
  rootNode->GetCollisions();
}

int QuadTree::GetSize() {
  return rootNode->Size();
}

QuadTree::QuadTreeNode::QuadTreeNode() {
  isLeaf = true;
  size = 0;
  parent = NULL;
}

QuadTree::QuadTreeNode::QuadTreeNode(glm::vec3 c, float h, float w, QuadTree::QuadTreeNode *_parent) {
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
  parent = _parent;
}

QuadTree::QuadTreeNode::QuadTreeNode(Collidable *entity, glm::vec3 c, float h, float w, QuadTree::QuadTreeNode *_parent) {
  isLeaf = true;
  bucket = new QuadTreeNode::EntityLinkedList(entity, 0);
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
  parent = _parent;
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

Collidable * QuadTree::QuadTreeNode::GetEntity (int index) const {
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

int QuadTree::QuadTreeNode::GetIndex (Collidable *target) const {
  EntityLinkedList *csr = bucket;
  int i = 0;
  while (csr != NULL) {
    if (csr->Get() == target) {
      return i;
    }
    csr = csr->Next();
    ++i;
  }
  return -1;
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

bool QuadTree::QuadTreeNode::InScope(Collidable *target) const {
  return target->CheckBounds(left, right, top, bottom);
}

bool QuadTree::QuadTreeNode::Contains(Collidable *object) {
  bool there = contains.find(object) != contains.end() && contains[object] != NULL;
  return there;
}

void QuadTree::QuadTreeNode::AddEntity(Collidable *newEntity) {
  // If it already contains the element, we need to figure out who owns it
  // And update its position / ownership
  if (Contains(newEntity)) {
    // Get the Node that contains that element
    QuadTree::QuadTreeNode *container = contains[newEntity];

    // Only perform checks if it did update
    if (!(newEntity->DidUpdate())) return;
    
    if (!isLeaf && container != this) {
      // Check containing node to make sure its right
      container->AddEntity(newEntity);
      return;
    }
    else {
      if (newEntity->CheckBounds(left, right, top, bottom)) {
	// Is it still in the current quadrant's bounds?
	if (!isLeaf) {
	  // Did it move into a child leaf?
	  // TODO: Optimize
	  QuadTreeNode *moveTo = NULL;
	  if (topLeftChild->InScope(newEntity)) moveTo = topLeftChild;
	  else if (topRightChild->InScope(newEntity)) moveTo = topRightChild;
	  else if (bottomLeftChild->InScope(newEntity)) moveTo = bottomLeftChild;
	  else if (bottomRightChild->InScope(newEntity)) moveTo = bottomRightChild;

	  if (moveTo != NULL) {
	    contains[newEntity] = moveTo;
	    // Remove from this node's unique items and move downwards
	    EntityLinkedList *csr = bucket;
	    while (csr != NULL) {
	      if (csr->Get() == newEntity) {
		csr->Get();
		if (csr->Prev() != NULL) {
		  csr->Prev()->RelinkNext(csr->Next());
		}
		else {
		  bucket = csr->Next();
		}
		if (csr->Next() != NULL) {
		  csr->Next()->RelinkPrev(csr->Prev());
		}
		delete csr;
		--size;
	      }
	      csr = csr->Next();
	    }
	    moveTo->AddEntity(newEntity);
	  }
	}
	return;
      }
      else {
	// Add to parent
	parent->PopEntity(newEntity);
	parent->AddEntity(newEntity);
	return;
      }
    }
  }
  // It is a new Entity, let's add it!
  ++totalSize; // this will happen no matter what
  int flagBuilder = 0;
  // What to do if it IS a leaf
  if (isLeaf) {
    // Add to this quadrant
    bucket = new EntityLinkedList(newEntity, flagBuilder, bucket);
    ++size;
    contains[newEntity] = this;

    // If the quadrant is too full, break it down into four children
    if (size > QUADTREE_THRESHOLD) {
      Decompose();
    }
    return;
  }
  // What to do if it is NOT a leaf
  else {
    // Check if it's in the top half of the quadrant
    if (newEntity->CheckBounds(left, right, top, center[1])) {
      flagBuilder |= 0b1100;
      if (newEntity->CheckBounds(left, center[0], top, center[1])) {
	// It belongs to the TL quadrant
	topLeftChild->AddEntity(newEntity);
	contains[newEntity] = topLeftChild;
	return;
      }
      else if (newEntity->CheckBounds(center[0], right, top, center[1])) {
	// It belongs to the TR quadrant
	topRightChild->AddEntity(newEntity);
	contains[newEntity] = topRightChild;
	return;
      }
    }
    // Check if its in the bottom half of the quadrant
    else if (newEntity->CheckBounds(left, right, center[1], bottom)) {
      flagBuilder |= 0b0011;
      if (newEntity->CheckBounds(left, center[0], center[1], bottom)) {
	// It belongs to the BL quadrant
	bottomLeftChild->AddEntity(newEntity);
	contains[newEntity] = bottomLeftChild;
	return;
      }
      else if (newEntity->CheckBounds(center[0], right, center[1], bottom)) {
	// It belongs to the BR quadrant
	bottomRightChild->AddEntity(newEntity);
	contains[newEntity] = bottomRightChild;
	return;
      }
    }
    // If we've gotten here, it does not belong anywhere...
    // Add to this node
    if ((flagBuilder & 0b1111) == 0) {
      // If not flags were added, figure it out
      if (newEntity->CheckBounds(left, center[0], top, bottom)) {
	flagBuilder |= 0b1010;
      }
      else if (newEntity->CheckBounds(left, center[0], top, bottom)) {
	flagBuilder |= 0b0101;
      }
      else {
	flagBuilder |= 0b1111;
      }
    }
    bucket = new EntityLinkedList(newEntity, flagBuilder, bucket);
    ++size;
    contains[newEntity] = this;
  }
}

// Use this function to recurse downwards
Collidable * QuadTree::QuadTreeNode::PopEntity (Collidable * target) {
  Collidable *output = NULL;
  if (contains[target] == this || isLeaf) {
    // Belongs to this node
    // Bubble upwards if it has a parent and it belongs to said parent
    if (parent != NULL && parent->Contains(target)) {
      return parent->PopEntity(target);
    }
    EntityLinkedList *csr = bucket;
    while (csr != NULL) {
      if (csr->Get() == target) {
	output = csr->Get();
	if (csr->Prev() != NULL) {
	  csr->Prev()->RelinkNext(csr->Next());
	}
	else {
	  bucket = csr->Next();
	}
	if (csr->Next() != NULL) {
	  csr->Next()->RelinkPrev(csr->Prev());
	}
	delete csr;
	--size;
	--totalSize;
	// Remove from this map
	contains.erase(contains.find(target));
	return output;
      }
      csr = csr->Next();
    }
    return output;
  }
  else if (contains[target] != NULL) {
    QuadTreeNode *child = contains[target];
    // Belongs to another node
    contains.erase(contains.find(target));
    --totalSize; // Reduce total size
    output = child->PopEntity(target);
  }
  return output;
}

// THIS FUNCTION ONLY WORKS ON ENTITIES BELONGING TO THIS ENTITY
Collidable * QuadTree::QuadTreeNode::PopEntity (int index) {
  Collidable *output = NULL;
  EntityLinkedList *csr = bucket;
  int i = 0;
  while (csr != NULL && i < index) {
    if (i == index) {
      output = csr->Get();
      if (csr->Prev() != NULL) {
	csr->Prev()->RelinkNext(csr->Next());
      }
      else {
	bucket = csr->Next();
      }
      if (csr->Next() != NULL) {
	csr->Next()->RelinkPrev(csr->Prev());
      }
      delete csr;
      --size;
      --totalSize;
      contains.erase(contains.find(output));
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
  float halfHeight = std::fabs((top - bottom) / 2.0f);
  float halfWidth = std::fabs((right - left) / 2.0f);
  glm::vec3 adjustment = glm::vec3(halfWidth / 2.0f, halfHeight / 2.0f, 0.0f);

  glm::vec3 tl = center + adjustment * glm::vec3(-1.0f, 1.0f, 1.0f);
  glm::vec3 tr = center + adjustment * glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 bl = center + adjustment * glm::vec3(-1.0f, -1.0f, 1.0f);
  glm::vec3 br = center + adjustment * glm::vec3(1.0f, -1.0f, 1.0f);
  
  topLeftChild = new QuadTreeNode(tl, halfHeight, halfWidth, this);
  topRightChild = new QuadTreeNode(tr, halfHeight, halfWidth, this);
  bottomLeftChild = new QuadTreeNode(bl, halfHeight, halfWidth, this);
  bottomRightChild = new QuadTreeNode(br, halfHeight, halfWidth, this);

  // Reassign all entities
  totalSize = 0;
  size = 0;
  contains.clear();
  EntityLinkedList *l = bucket;
  EntityLinkedList *g = bucket;
  bucket = NULL;
  while (l != NULL) {
    AddEntity(l->Get());
    l = l->Next();
    // Clean up the old list
    delete g;
    g = l;
  }
}

int QuadTree::QuadTreeNode::Size () const {
  if (!isLeaf) std::cout << "TOTAL: " << totalSize << std::endl;
  std::cout << "Mine: " << SizeGetUnique() << std::endl;
  if (isLeaf) return totalSize;
  std::cout << "TL: ";
  topLeftChild->Size();
  std::cout << "TR: ";
  topRightChild->Size();
  std::cout << "BL: ";
  bottomLeftChild->Size();
  std::cout << "BR: ";
  bottomRightChild->Size();
  return totalSize;
}

int QuadTree::QuadTreeNode::SizeGetUnique () const {
  return size;
}

// THE MAIN REASON FOR THIS DAMN DS
void QuadTree::QuadTreeNode::GetCollisions () {
  if (totalSize < 2) {
    // No collisions are possible
    return;
  }
  if (size > 0) {
    // TODO: Implement Sensor Checking...
    std::vector<std::pair<Collidable *, Collidable *> > checked;
    checked.reserve(size * 10);
    
    EntityLinkedList *csr = bucket;
    EntityLinkedList *next = NULL;
    Collidable *curr;
    bool dead = false;
    while (csr != NULL) {
      next = csr->Next();
      curr = csr->Get();
      int bordered = csr->GetBorderFlags();

      // Check with other elements in this node
      csr = bucket;
      while (csr != NULL) {
	// No reflexive checks
	if (csr->Get() == curr) {
	  csr = csr->Next();
	  continue;
	}
	// See if this edge has been checked already
	bool skip = false;
	std::pair<Collidable *, Collidable *> edge = std::make_pair(curr, csr->Get());
	std::pair<Collidable *, Collidable *> redge = std::make_pair(csr->Get(), curr);
	for (int i = 0; i < checked.size(); ++i) {
	  if (checked[i] == edge || checked[i] == redge) {
	    // Its been checked already
	    skip = true;
	    break;
	  }
	}
	if (skip) {
	  csr = csr->Next();
	  continue;
	}
	int collisionStatus = curr->CheckCollision(csr->Get());

	checked.push_back(edge);
	checked.push_back(redge);

	if (collisionStatus == QUADTREE_DEAD_ENTITY) {
	  PopEntity(curr);
	  dead = true;
	  break;
	}
	else if (collisionStatus == QUADTREE_YES_COLLISION) {
	  curr->DidCollide(csr->Get());
	  csr->Get()->DidCollide(curr);
	}
      }

      if (dead) {
	csr = next;
	continue;
      }

      // If not a leaf, check this with ones in bordering quadrants
      if (!isLeaf) {
	if (bordered & 0b1000) topLeftChild->GetCollisions(curr, 0b1000, bordered);
	if (bordered & 0b0100) topRightChild->GetCollisions(curr, 0b0100, bordered);
	if (bordered & 0b0010) bottomLeftChild->GetCollisions(curr, 0b0010, bordered);
	if (bordered & 0b0001) bottomRightChild->GetCollisions(curr, 0b0001, bordered);
      }

      csr = next;
    }
  }
  
  if (!isLeaf) {
    // If its a leaf, check the children...
    topLeftChild->GetCollisions();
    topRightChild->GetCollisions();
    bottomLeftChild->GetCollisions();
    bottomRightChild->GetCollisions();
  }
}

void QuadTree::QuadTreeNode::GetCollisions(Collidable *e, int pos, int borderFlag) {
  if (totalSize == 0 || e == NULL) {
    return;
  }
  // If it is a leaf, this is an easy process
  
  // Check every Entity
  EntityLinkedList *csr = bucket;
  while (csr != NULL) {
    int collisionStatus = e->CheckCollision(csr->Get());
    if (collisionStatus == QUADTREE_YES_COLLISION) {
      e->DidCollide(csr->Get());
      csr->Get()->DidCollide(e);
    }
    else if (collisionStatus == QUADTREE_DEAD_ENTITY) {
      parent->PopEntity(e);
      break;
    }
    csr = csr->Next();
  }
  // Else if not a leaf,
  // Check for overlapping entities on appropriate side
  // Then bubble downwards to bordering quadrants
  if (!isLeaf) {
    // TODO: Optimize this function
    topLeftChild->GetCollisions(e, 0, 0);
    topRightChild->GetCollisions(e, 0, 0);
    bottomLeftChild->GetCollisions(e, 0, 0);
    bottomRightChild->GetCollisions(e, 0, 0);
  }
}

int QuadTree::QuadTreeNode::GetBorderedQuadrants(Collidable *e) {
  if (e->CheckBounds(left, center[0], top, bottom)) {
    return 0b1010;
  }
  if (e->CheckBounds(center[0], right, top, bottom)) {
    return 0b0101;
  }
  if (e->CheckBounds(left, right, top, center[1])) {
    return 0b1100;
  }
  if (e->CheckBounds(left, right, center[1], bottom)) {
    return 0b0011;
  }
  return 0b1111;
}

QuadTree::QuadTreeNode::EntityLinkedList::EntityLinkedList(Collidable *e, int flags) {
  entity = e;
  after = NULL;
  before = NULL;
  borderFlags = flags;
}

QuadTree::QuadTreeNode::EntityLinkedList::EntityLinkedList(Collidable *e, int flags, EntityLinkedList *next) {
  entity = e;
  after = next;
  before = NULL;
  borderFlags = flags;
  if (next != NULL) next->RelinkPrev(this);
}

Collidable * QuadTree::QuadTreeNode::EntityLinkedList::Get() const {
  return entity;
}

int QuadTree::QuadTreeNode::EntityLinkedList::GetBorderFlags() const {
  return borderFlags;
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
