#include "Pi2Jamma/screens/ScreenController.hpp"

#include "Pi2Jamma/screens/ScreenControllerStack.hpp"

ScreenController::ScreenController()
    : m_pScreenControllerStack{nullptr}
{
}

ref<ui::Element> ScreenController::activate(NotNullPtr<ScreenControllerStack> nnpScreenControllerStack)
{
    ASSERT(!m_pScreenControllerStack);
    m_pScreenControllerStack = nnpScreenControllerStack;
    
    return
        onActivate(
            m_pScreenControllerStack->getRootElement(),
            m_pScreenControllerStack->getRootElement()->getRect());
}

void ScreenController::deactivate()
{
    ASSERT(m_pScreenControllerStack);
    
    onDeactivate();
    m_pScreenControllerStack = nullptr;
}

 ScreenControllerStack& ScreenController::getScreenControllerStack()
 {
     ASSERT(m_pScreenControllerStack);
     return *m_pScreenControllerStack;
 }