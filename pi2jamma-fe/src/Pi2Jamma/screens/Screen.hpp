#pragma once

#include "core/NotNullPtr.hpp"
#include "core/RefCounted.hpp"
#include "Pi2Jamma/screens/ScreenModel.hpp"
#include "Pi2Jamma/screens/ScreenTheme.hpp"
#include "ui/BitmapFont.hpp"
#include "ui/elements/Label.hpp"
#include "ui/elements/Image.hpp"
#include "ui/elements/TextList.hpp"

class Screen : public ui::Element
{
public:
    Screen(
        ui::Element* pParent,
		const ui::Rect& rect,
        ref<ScreenTheme>,
		NotNullPtr<ScreenModel>);

private:

	class ListModel : public ui::TextListModel
	{
	public:
		ListModel(Screen& screen);
	private:
		virtual size_t getNumItems() override;
		virtual CStr getItem(size_t index) override;
		virtual void onHighlighted(size_t newSelection) override;
		virtual void onSelect(size_t newSelection) override;

		Screen& m_screen;
	};

	ListModel m_textListModel;
	
	ref<ScreenTheme> m_refTheme;
	NotNullPtr<ScreenModel> m_nnpModel;

	ref<ui::Image> m_refBackgroundImage;
	ref<ui::Label> m_refTitle;
	ref<ui::TextList> m_refTextList;
	ref<ui::Image> m_refSnapsImage;
};