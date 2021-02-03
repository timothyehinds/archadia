#pragma once

#include "core/CString.hpp"
#include "ui/Surface.hpp"

class ScreenModel
{
public:
    struct Item
    {
        CStr m_itemText;
        ref<ui::Surface> m_refSnapsTexture;
    };

    virtual CStr getTitle() = 0;
    virtual size_t getNumItems() = 0;
    virtual Item getItem(size_t) = 0;
    virtual void itemSelected(size_t) = 0;

};