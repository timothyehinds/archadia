#pragma once

#include "core/NotNullPtr.hpp"
#include "ui/elements/Element.hpp"

class ScreenControllerStack;

class ScreenController
{
public:
    ScreenController();
    virtual ~ScreenController() = default;

    ref<ui::Element>  activate(NotNullPtr<ScreenControllerStack> nnpScreenControllerStack);
    void deactivate();

protected:
    virtual ref<ui::Element> onActivate(ui::Element* pParent, const ui::Rect& rect) = 0;
    virtual void onDeactivate() = 0;

    ScreenControllerStack& getScreenControllerStack();

private:
    ScreenControllerStack* m_pScreenControllerStack;
};