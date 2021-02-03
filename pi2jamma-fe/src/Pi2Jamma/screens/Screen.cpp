#include "Pi2Jamma/screens/Screen.hpp"

Screen::ListModel::ListModel(Screen& screen)
	: m_screen(screen)
{

}
size_t Screen::ListModel::getNumItems()
{
	return m_screen.m_nnpModel.get().getNumItems();
}

CStr Screen::ListModel::getItem(const size_t index)
{
	return m_screen.m_nnpModel.get().getItem(index).m_itemText;
}

void Screen::ListModel::onHighlighted(const size_t index)
{
	m_screen.m_refSnapsImage->setSurface(m_screen.m_nnpModel.get().getItem(index).m_refSnapsTexture);
}

void Screen::ListModel::onSelect(size_t newSelection)
{
}

Screen::Screen(
    ui::Element* pParent,
    const ui::Rect& rect,
    ref<ScreenTheme> refTheme,
	NotNullPtr<ScreenModel> nnpScreenModel)
    : ui::Element(pParent, rect)
	, m_textListModel{*this}
    , m_refTheme{std::move(refTheme)}
	, m_nnpModel{std::move(nnpScreenModel)}
{
		/*
   std::string configFilePath =
		joinPath(fullThemeDir, "config.txt");

	Result result = loadJson(mTheme, configFilePath.c_str());
	result.catastrophic(); */

	const ScreenTheme& theme{*m_refTheme};

	const ScreenThemeDescription& themeDescription{
		theme.getDescription()};	
	
	m_refBackgroundImage =
		make_ref<ui::Image>(
			this,
			getRect(),
			theme.getBackgroundSurface());			

	m_refSnapsImage =
		make_ref<ui::Image>(
			this,
			themeDescription.getSnapsRect());
	
	m_refTitle =
		make_ref<ui::Label>(
			this,
			themeDescription.getTitleRect(),
			theme.getTitleBitmapFont(),
			themeDescription.getTitleTextColor(),
			"Title",
			themeDescription.getTitleAlignment());

	m_refTextList =
		make_ref<ui::TextList>(
			m_textListModel,
			this,
			themeDescription.getMenuRect(),
			theme.getSelectedBitmapFont(),
			theme.getUnselectedBitmapFont(),
			themeDescription.getMenuTextColor(),
			themeDescription.getMenuTextHighlightColor(),
			themeDescription.getMenuTextSize());
}


