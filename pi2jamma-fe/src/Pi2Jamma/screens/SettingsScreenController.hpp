#pragma once

#include "Pi2Jamma/screens/Screen.hpp"
#include "Pi2Jamma/screens/ScreenController.hpp"
#include "Pi2Jamma/screens/ScreenModel.hpp"

class SettingsScreenController : public ScreenController
{
public:
    SettingsScreenController(ref<ScreenTheme>);

    SettingsScreenController(const SettingsScreenController&) = delete;
    SettingsScreenController& operator=(const SettingsScreenController&) = delete;

    virtual ref<ui::Element> onActivate(ui::Element* pParent, const ui::Rect& rect) override;
    virtual void onDeactivate() override;
private:

    class SettingsScreenModel : public ScreenModel
    {
    public:
        SettingsScreenModel(NotNullPtr<SettingsScreenController>);

        virtual CStr getTitle() override;
        virtual size_t getNumItems() override;
        virtual CStr getItemText(size_t) override;
        virtual ref<ui::Surface> getItemSurface(size_t) override;
        virtual void onItemSelected(size_t) override;    
    private:
        NotNullPtr<SettingsScreenController> m_nnpController;
    };

    SettingsScreenModel m_screenModel;

    ref<Screen> m_refScreen;    
    ref<ScreenTheme> m_refScreenTheme;
};