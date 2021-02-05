#include "Pi2Jamma/screens/ScreenTheme.hpp"

#include "ui/Application.hpp"

#include <filesystem>

namespace fs = std::filesystem;

ScreenTheme::ScreenTheme(
    ScreenThemeDescription themeDescription,
    const CStr fullThemeDir)
    : m_description{std::move(themeDescription)}
{
	Result<ref<ui::Font>> resultFont{
        ui::Application::get().loadFont(
			m_description.getMenuTextSize(),
			(fs::path(fullThemeDir.c_str()) / m_description.getFontFilePath().c_str()).c_str())};
	
	resultFont.catastrophic();

	m_refFont = *resultFont;
	
	m_refSelectedBitmapFont =
		ui::BitmapFont::fromFont(
			m_refFont,
			m_description.getMenuTextHighlightColor());

	m_refUnselectedBitmapFont =
		ui::BitmapFont::fromFont(
			m_refFont,
			m_description.getMenuTextColor());  

	Result<ref<ui::Surface>> resultSurface{
		ui::Application::get().loadSurface(
			(fs::path(fullThemeDir.c_str()) / "background.png").c_str())};
	
	resultSurface.catastrophic();

	m_refBackgroundSurface = *resultSurface;
}
