#pragma once

template<
    typename ItemType,
    typename NodeType,
    NodeType ItemType::*Member >
struct NodeFinderField
{
    static size_t offset() {

        NodeType& member = ((ItemType*)(nullptr))->*Member;

        const size_t o =
            reinterpret_cast<char*>(&member)
                - reinterpret_cast<char*>((ItemType*)(nullptr));   

        return o;   
    }

    static ItemType& fromNode(NodeType& node) {
        return
            *reinterpret_cast<ItemType*>(
                const_cast<char*>(
                    reinterpret_cast<const char*>(&node) - offset()));
    }

    static NodeType& toNode(ItemType& itemType) {
        return *((NodeType*)(& (itemType.*Member)));
    }
};