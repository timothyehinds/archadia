#include "Pi2Jamma/screens/SettingsScreenController.hpp"

#include "core/system.hpp"

SettingsScreenController::SettingsScreenModel::SettingsScreenModel(
    NotNullPtr<SettingsScreenController> nnpController)
    : m_nnpController{nnpController}
{
}

CStr SettingsScreenController::SettingsScreenModel::getTitle()
{
    return "Settings";
}

size_t SettingsScreenController::SettingsScreenModel::getNumItems()
{
    return 2;
}

ScreenModel::Item SettingsScreenController::SettingsScreenModel::getItem(
    const size_t i)
{
    if (0 == i)
    {
        return ScreenModel::Item{"Launch Retroarch"};
    }

    return ScreenModel::Item{"Screen Orientation"};
}

void SettingsScreenController::SettingsScreenModel::itemSelected(size_t)
{
    //System::exec("retroarch");
}

//------------------------------------------------------------------------------

SettingsScreenController::SettingsScreenController(
    ui::Element* pParent,
    ref<ScreenTheme> refScreenTheme)
    : m_screenModel{*this}
{
    m_refScreen =
        make_ref<Screen>(
            pParent,
            pParent->getRect(),
            refScreenTheme,
            m_screenModel);
            
}

