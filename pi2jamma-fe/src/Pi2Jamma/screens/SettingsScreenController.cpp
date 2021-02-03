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

CStr SettingsScreenController::SettingsScreenModel::getItemText(const size_t i)
{
    if (0 == i)
    {
        return "Launch Retroarch";
    }

    return "Screen Orientation";
}

ref<ui::Surface> SettingsScreenController::SettingsScreenModel::getItemSurface(const size_t i)
{
    return nullptr;
}

void SettingsScreenController::SettingsScreenModel::onItemSelected(const size_t)
{
    std::string output;
    Result r{System::exec("retroarch -c ../data/retroarch.cfg", output)};
    r.ignore();
}

//------------------------------------------------------------------------------

SettingsScreenController::SettingsScreenController(
    ref<ScreenTheme> refScreenTheme)
    : m_screenModel{*this}
    , m_refScreenTheme{std::move(refScreenTheme)}
{
        
}

ref<ui::Element> SettingsScreenController::activate(ui::Element* pParent, const ui::Rect& rect)
{
    m_refScreen =
        make_ref<Screen>(
            pParent,
            rect,
            m_refScreenTheme,
            m_screenModel);

    return m_refScreen;
}

void SettingsScreenController::deactivate()
{
    m_refScreen = nullptr;
}

