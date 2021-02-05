#include "Pi2Jamma/screens/ScreenControllerStack.hpp"

ScreenControllerStack::ScreenControllerStack(ref<ui::Element> refRootElement)
    : m_refRootElement{std::move(refRootElement)}
{
}

ref<ui::Element> ScreenControllerStack::getRootElement()
{
    return m_refRootElement;
}

void ScreenControllerStack::push(
    std::unique_ptr<ScreenController>&& uptScreenController)
{
    if (!(m_screenControllerVector.empty()))
    {
        m_screenControllerVector.back()->deactivate();
    }

    m_refActiveElement = uptScreenController->activate(*this);
    m_screenControllerVector.push_back(std::move(uptScreenController));
}

bool ScreenControllerStack::pop()
{
    ASSERT(!(m_screenControllerVector.empty()));

    m_screenControllerVector.back()->deactivate();

    m_screenControllerVector.pop_back();

    if (!(m_screenControllerVector.empty()))
    {
        m_refActiveElement = m_screenControllerVector.back()->activate(*this);
        return true;
    }

    m_refActiveElement = nullptr;
    return false;
}