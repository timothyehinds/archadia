#pragma once

#include "Pi2Jamma/screens/Screen.hpp"
#include "Pi2Jamma/screens/ScreenModel.hpp"

class SettingsScreenController
{
public:
    SettingsScreenController(
        ui::Element* pParent,
        ref<ScreenTheme>);

    SettingsScreenController(const SettingsScreenController&) = delete;
    SettingsScreenController& operator=(const SettingsScreenController&) = delete;
private:

    class SettingsScreenModel : public ScreenModel
    {
    public:
        SettingsScreenModel(NotNullPtr<SettingsScreenController>);

        virtual CStr getTitle() override;
        virtual size_t getNumItems() override;
        virtual Item getItem(size_t) override;
        virtual void itemSelected(size_t) override;    
    private:
        NotNullPtr<SettingsScreenController> m_nnpController;
    };

    SettingsScreenModel m_screenModel;

    ref<Screen> m_refScreen;    
};