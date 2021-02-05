#pragma once

#include "Pi2Jamma/screens/ScreenController.hpp"

#include "ui/elements/Element.hpp"

#include <vector>

class ScreenControllerStack
{
public:
    ScreenControllerStack(ref<ui::Element> refRootElement);

    void push(std::unique_ptr<ScreenController>&&);
    bool pop();

    ref<ui::Element> getRootElement();
private:
    std::vector<std::unique_ptr<ScreenController>> m_screenControllerVector;
    ref<ui::Element> m_refRootElement;
    ref<ui::Element> m_refActiveElement;
};