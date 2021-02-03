#pragma once

#include "ui/elements/Element.hpp"

class ScreenController
{
public:
    virtual ref<ui::Element> activate(ui::Element* pParent, const ui::Rect& rect) = 0;
    virtual void deactivate() = 0;

    virtual ~ScreenController() = default;
};