#pragma once

#include "Pi2Jamma/screens/ScreenController.hpp"

#include "ui/elements/Element.hpp"

#include <vector>

class ScreenControllerStack
{
public:
    void push(ui::Element* pParent, const ui::Rect& rect, std::unique_ptr<ScreenController>&&);
    void pop(ui::Element* pParent, const ui::Rect& rect);
private:
    std::vector<std::unique_ptr<ScreenController>> m_screenControllerVector;
    ref<ui::Element> m_refActiveElement;
};