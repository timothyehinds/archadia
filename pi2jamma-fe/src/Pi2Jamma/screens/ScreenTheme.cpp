#include "Pi2Jamma/screens/ScreenTheme.hpp"

#include "core/file/FilePath.hpp"
#include "ui/Application.hpp"

ScreenTheme::ScreenTheme(
    ScreenThemeDescription themeDescription,
    const CStr fullThemeDir)
    : m_description{std::move(themeDescription)}
{
	Result result{
        ui::Application::get().loadFont(
			m_refFont,
			m_description.getMenuTextSize(),
			joinPath(fullThemeDir, m_description.getFontFilePath()))};
	
	result.catastrophic();
	
	m_refSelectedBitmapFont =
		ui::BitmapFont::fromFont(
			m_refFont,
			m_description.getMenuTextHighlightColor());

	m_refUnselectedBitmapFont =
		ui::BitmapFont::fromFont(
			m_refFont,
			m_description.getMenuTextColor());  

	result =
		ui::Application::get().loadSurface(
			m_refBackgroundSurface,
			joinPath(fullThemeDir, "background.png"));
	
	result.catastrophic();
}
