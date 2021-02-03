#pragma once

#include "core/CString.hpp"
#include "ui/Surface.hpp"

class ScreenModel
{
public:
    virtual CStr getTitle() = 0;
    virtual size_t getNumItems() = 0;
    virtual CStr getItemText(size_t) = 0;
    virtual ref<ui::Surface> getItemSurface(size_t) = 0;
    virtual void onItemSelected(size_t) = 0;
};