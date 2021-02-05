#include "Pi2Jamma/screens/SettingsScreenController.hpp"

#include "core/system.hpp"

#include "Pi2Jamma/screens/ScreenControllerStack.hpp"
#include "Pi2Jamma/screens/SettingsScreenController.hpp"
#include "Pi2Jamma/screens/GameSelectScreenController.hpp"

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

CStr SettingsScreenController::SettingsScreenModel::getItemText(const size_t i)
{
    if (0 == i)
    {
        return "Games";
    }
    else if (1 == i)
    {
        return "Launch Retroarch";
    }

    return "";
}

ref<ui::Surface> SettingsScreenController::SettingsScreenModel::getItemSurface(const size_t i)
{
    return nullptr;
}

void SettingsScreenController::SettingsScreenModel::onItemSelected(const size_t i)
{
    if (0 == i)
    {
        m_nnpController.get().getScreenControllerStack().push(
            std::make_unique<GameSelectScreenController>(
                m_nnpController.get().m_refScreenTheme));
    }
    else if (1 == i)
    {
        std::string output;
        Result<Success> r{System::exec("retroarch -c ../data/retroarch.cfg", output)};
    }
}

//------------------------------------------------------------------------------

SettingsScreenController::SettingsScreenController(
    ref<ScreenTheme> refScreenTheme)
    : m_screenModel{*this}
    , m_refScreenTheme{std::move(refScreenTheme)}
{
        
}

ref<ui::Element> SettingsScreenController::onActivate(ui::Element* pParent, const ui::Rect& rect)
{
    m_refScreen =
        make_ref<Screen>(
            pParent,
            rect,
            m_refScreenTheme,
            m_screenModel);

    return m_refScreen;
}

void SettingsScreenController::onDeactivate()
{
    m_refScreen = nullptr;
}

