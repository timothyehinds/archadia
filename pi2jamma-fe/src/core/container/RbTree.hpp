#pragma once

#include "core/container/RbTreeBase.hpp"
#include "core/container/KeyFinder.hpp"
#include "core/container/LifetimePolicy.hpp"
#include "core/container/NodeFinder.hpp"
#include "core/Compare.hpp"

template<
    typename ItemType,
    typename KeyType,
    typename KeyFinderType,
    typename NodeFinderType,
    typename LifetimePolicy = LifetimePolicyNone<ItemType>,
    bool AllowDuplicates = false >
class RbTree final
{
public:

    ~RbTree() {
        clear();
    }

    void insert(ItemType& item) {
        mTree.insert(
            & NodeFinderType::toNode(item),
            [](RbTreeNode* n1, RbTreeNode* n2) {
                return
                    Comparer<KeyType>::Compare(
                        KeyFinderType::get((NodeFinderType::fromNode(*n1))),
                        KeyFinderType::get((NodeFinderType::fromNode(*n2))));

            } );

        LifetimePolicy::addRef(item);
    }

    ItemType* find(typename Arg<KeyType>::Type key) const {
        RbTreeNode* pNodeBase = mTree.find(
            [&key](RbTreeNode* rhs) {
                return
                    Comparer<KeyType>::Compare(
                        KeyFinderType::get((NodeFinderType::fromNode(*rhs))),
                        key);
            });

        if(nullptr == pNodeBase) {
            return nullptr;
        }

        return & NodeFinderType::fromNode(*pNodeBase);
    }

    void remove(ItemType& item) {
        mTree.remove(& NodeFinderType::toNode(item));
        LifetimePolicy::release(item);
    }

    void clear() {

        #ifdef DEBUG_CONTAINER
            RbTreeNode* pI = mTree.getFirst();
            while(nullptr != pI) {
                RbTreeNode* pNext = mTree.getNext(pI);
                mTree.remove(pI);
                LifetimePolicy::release(NodeFinderType::fromNode(*pI));
                pI = pNext;
            }
        #else

            for(auto&& i: *this) {
                LifetimePolicy::release(i);
            }

        #endif

        mTree.clear();
    }


    class Iterator {
        public:
            Iterator(
                const RbTree* pTree,
                RbTreeNode* pNode)
                : mpTree(pTree)
                , mpNode(pNode) {
            }

            bool operator !=(Iterator& rhs) {
                return mpNode != rhs.mpNode;
            }

            Iterator& operator++() {
                ASSERT(nullptr != mpNode);
                mpNode = mpTree->mTree.getNext(mpNode);
                return *this;
            }

            ItemType& operator*() {
                ASSERT(nullptr != mpNode);
                return NodeFinderType::fromNode(*mpNode);
            }

        private:

            const RbTree* mpTree;
            RbTreeNode* mpNode;            
    };

    Iterator begin() const {
        return Iterator(this, mTree.getFirst());
    }

    Iterator end() const {
        return Iterator(this, nullptr);
    }

private:

    RbTreeBase mTree;
};
