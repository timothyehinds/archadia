#pragma once

#include "core/container/DlListBase.hpp"
#include "core/container/NodeFinder.hpp"

template<typename ItemType, typename NodeFinderType>
class DlList final
{
public:

	class Iterator
	{
	public:
		Iterator& operator++() {
			mpNode = mpList->mList.getNext(mpNode);
			return *this;
		}

		bool operator !=(const Iterator& rhs) const{
			return mpNode != rhs.mpNode;
		}

		ItemType& operator*() {
			ASSERT(nullptr != mpNode);
			return NodeFinderType::fromNode(*mpNode);
		}

	private:
		Iterator(const DlList* pList, DlListNode* pNode)
			: mpList(pList)
			, mpNode(pNode) {}	

		const DlList* mpList;
		DlListNode* mpNode;

	friend class DlList;

	};

	DlList();
	~DlList();

	void insertHead(ItemType& item);
	void insertTail(ItemType& item);
	void remove(ItemType& item);

	ItemType* getHead() const;
	ItemType* getTail() const;

	ItemType* getNext(ItemType& prev) const;
	ItemType* getPrev(ItemType& next) const;

	Iterator begin() const;
	Iterator end() const;

private:

	ItemType* toItemPtr(DlListNode* pNode) const;
	DlListBase mList;
};

template<typename ItemType, typename NodeFinderType>
DlList<ItemType, NodeFinderType>::DlList()
{
}

template<typename ItemType, typename NodeFinderType>
DlList<ItemType, NodeFinderType>::~DlList()
{
}

template<typename ItemType, typename NodeFinderType>
void DlList<ItemType, NodeFinderType>::insertHead(ItemType& item)
{
	mList.insertHead(&NodeFinderType::toNode(item));
}

template<typename ItemType, typename NodeFinderType>
void DlList<ItemType, NodeFinderType>::insertTail(ItemType& item)
{
	mList.insertTail(&NodeFinderType::toNode(item));
}

template<typename ItemType, typename NodeFinderType>
void DlList<ItemType, NodeFinderType>::remove(ItemType& item)
{
	mList.remove(&NodeFinderType::toNode(item));
}

template<typename ItemType, typename NodeFinderType>
ItemType* DlList<ItemType, NodeFinderType>::getHead() const
{
	return toItemPtr(mList.getHead());
}

template<typename ItemType, typename NodeFinderType>
ItemType* DlList<ItemType, NodeFinderType>::getTail() const
{
	return toItemPtr(mList.getTail());
}

template<typename ItemType, typename NodeFinderType>
ItemType* DlList<ItemType, NodeFinderType>::getNext( 
	ItemType& prev) const
{
	return toItemPtr(mList.getNext(&NodeFinderType::toNode(prev)));
}

template<typename ItemType, typename NodeFinderType>
ItemType* DlList<ItemType, NodeFinderType>::getPrev(ItemType& next) const
{
	return toItemPtr(mList.getPrev(&NodeFinderType::toNode(next)));
}

template<typename ItemType, typename NodeFinderType>
ItemType* DlList<ItemType, NodeFinderType>::toItemPtr(DlListNode* pNode) const
{
	if(nullptr == pNode) {
		return nullptr;
	}

	return & NodeFinderType::fromNode(pNode);
}

template<typename ItemType, typename NodeFinderType>
typename DlList<ItemType, NodeFinderType>::Iterator DlList<ItemType, NodeFinderType>::begin() const
{
	return Iterator(this, mList.getHead());
}

template<typename ItemType, typename NodeFinderType>
typename DlList<ItemType, NodeFinderType>::Iterator DlList<ItemType, NodeFinderType>::end() const
{
	return Iterator(this, nullptr);
}






