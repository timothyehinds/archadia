#include "Pi2Jamma/screens/ScreenControllerStack.hpp"

void ScreenControllerStack::push(
    ui::Element* pParent,
    const ui::Rect& rect,
    std::unique_ptr<ScreenController>&& uptScreenController)
{
    if (!(m_screenControllerVector.empty()))
    {
        m_screenControllerVector.back()->deactivate();
    }

    m_refActiveElement = uptScreenController->activate(pParent, rect);

    m_screenControllerVector.push_back(std::move(uptScreenController));
}

void ScreenControllerStack::pop(ui::Element* pParent, const ui::Rect& rect)
{
    ASSERT(!(m_screenControllerVector.empty()));

    m_screenControllerVector.back()->deactivate();

    m_screenControllerVector.pop_back();

    if (!(m_screenControllerVector.empty()))
    {
        m_refActiveElement = m_screenControllerVector.back()->activate(pParent, rect);
    }
    else
    {
        m_refActiveElement = nullptr;
    }
}