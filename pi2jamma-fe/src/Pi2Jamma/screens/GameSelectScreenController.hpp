#pragma once

#include "Pi2Jamma/screens/Screen.hpp"
#include "Pi2Jamma/screens/ScreenController.hpp"

class GameSelectScreenController : public ScreenController
{
public:
	GameSelectScreenController(
        ref<ScreenTheme>);

    virtual ref<ui::Element> activate(
		ui::Element* pParent,
		const ui::Rect&) override;

    virtual void deactivate() override;

private:
	class GameSelectScreenModel : public ScreenModel
	{
	public:
		GameSelectScreenModel(GameSelectScreenController&);

		virtual CStr getTitle() override;
		virtual size_t getNumItems() override;
		virtual CStr getItemText(size_t) override;
		virtual ref<ui::Surface> getItemSurface(size_t) override;
		virtual void onItemSelected(size_t) override;
		
		GameSelectScreenController& m_controller;
	};

	GameSelectScreenModel m_screenModel;
	ref<ScreenTheme> m_refScreenTheme;
	ref<Screen> m_refScreen;
};
