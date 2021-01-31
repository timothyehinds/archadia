#pragma once

#include <functional>
#include "core/container/Container.hpp"
#include "core/debug.hpp"

class RbTreeBase;

class RbTreeNode
{
public:
    #ifdef DEBUG_CONTAINER

        RbTreeNode() {
            mpTree = nullptr;
        }

        ~RbTreeNode() {
            ASSERT(nullptr == mpTree);
        }

    #endif

    RbTreeNode(const RbTreeNode& rhs) = delete;
    RbTreeNode& operator=(const RbTreeNode& rhs) = delete;

private:

    RbTreeNode* mpLeft;
    RbTreeNode* mpRight;
    RbTreeNode* mpParent;

    size_t mRed;    

    #ifdef DEBUG_CONTAINER
        mutable RbTreeBase* mpTree;
    #endif

friend class RbTreeBase;

};

class RbTreeBase
{
public:

	RbTreeBase();
	~RbTreeBase();

	using InsertFunction = const std::function<int(RbTreeNode*, RbTreeNode*)>;
	using CompareFunction = const std::function<int(RbTreeNode*)>;

	void clear();
	void insert(RbTreeNode* pTreeNode, const InsertFunction& insrtFunction);
	void remove(RbTreeNode* pTreeNode);

	RbTreeNode* find(const CompareFunction& compare) const;

    RbTreeNode* findClosest(const CompareFunction& compare) const;

	RbTreeNode* getFirst() const;

	RbTreeNode* getNext(RbTreeNode* pPrev) const;

	RbTreeNode* getLast() const;

	RbTreeNode* getPrev(RbTreeNode* pNext) const;

	size_t count() const;

	RbTreeNode* findAt(size_t index) const;

private:

	void reset();

	void insertHelp(RbTreeNode* pZ, const InsertFunction& insertFunction);
	void removeFixUp(RbTreeNode* pX);

	void leftRotate(RbTreeNode* pX);
	void rightRotate(RbTreeNode* pY);
	RbTreeNode* successor(RbTreeNode* pX) const;
	RbTreeNode* successor(RbTreeNode* pX);
	RbTreeNode* predecessor(RbTreeNode* pX) const;

    mutable RbTreeNode mRoot;
    mutable RbTreeNode mSentinal;	
};	
