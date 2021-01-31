#pragma once

#include "core/Arg.hpp"

template<typename ItemType, typename KeyType, typename Arg<KeyType>::Type (ItemType::*Getter)() const>
struct KeyFinderGetter
{
    static typename Arg<KeyType>::Type get(ItemType& item) {
        return (item.*Getter)();
    }
};