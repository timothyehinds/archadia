#include "Pi2Jamma/Pi2JammaApplication.hpp"

#include "Pi2Jamma/CommandLine/CommandLineHandlerConfigFile.hpp"
#include "Pi2Jamma/screens/ScreenTheme.hpp"
#include "Pi2Jamma/screens/SettingsScreenController.hpp"

#include "core/meta/Meta.hpp"
#include "core/json/JsonParser.hpp"
#include "core/serialize/json/JsonSerialize.hpp"
#include "core/CommandLine/CommandLine.hpp"
#include "ui/ui.hpp"
#include "ui/Point.hpp"

#include <filesystem>

namespace fs = std::filesystem;

Pi2JammaApplication::Pi2JammaApplication()
	: m_dataPath("./data")
{
}

Result<Success> Pi2JammaApplication::initialize(int argc, const char* argv[])
{
	Meta::initialize();
	ui::initialize();

	Configuration::initialize();
	ScreenThemeDescription::initialize();

	Result result = CommandLine::get().parse(argc, argv);
	if(!result) {
		return result;
	}

	result = loadConfiguration();
	if(!result) {
		return result;
	}

	return setupUi();
}

Result<Success> Pi2JammaApplication::loadConfiguration()
{
	Result<Success> result = loadJson(
		mConfiguration,
		CommandLineHandlerConfigFile::sSingleton.mConfigFile);

	if(!result)
	{
		return result;
	}

	const fs::path themesDir = m_dataPath / "themes";

	const UiConfiguration& uiConfig = mConfiguration.getUi();

	bool portrait =
		(ui::Orientation::Portrait == uiConfig.getScreenOrientation());

	CStr orientationDir =
		portrait
			? "portrait"
			: "landscape";

	CStr themeDir =
		portrait
			? uiConfig.getPortraitTheme()
			: uiConfig.getLandscapeTheme();

	// Load theme

	m_fullThemePath = themesDir / orientationDir.c_str() / themeDir.c_str();

	return Result{Success{}};
}

Result<Success> Pi2JammaApplication::setupUi()
{
	ui::Rect screenRect(
		ui::Point(0, 0),
		getScreenSize());

	m_refRootElement =
		make_ref<ui::Element>(
			nullptr,
			screenRect);

	m_uptScreenControllerStack =
		std::make_unique<ScreenControllerStack>(m_refRootElement.get());

	loadJson(m_screenThemeDescription, (m_fullThemePath / "config.txt").c_str()).catastrophic();

	ref<ScreenTheme> refScreenTheme{
		make_ref<ScreenTheme>(
			m_screenThemeDescription,
			m_fullThemePath.c_str())};

	std::unique_ptr<SettingsScreenController> uptSettingsScreenController{
		std::make_unique<SettingsScreenController>(refScreenTheme)};

	m_uptScreenControllerStack->push(
		std::move(uptSettingsScreenController));

	Result<ref<ui::Font>> resultFont = loadFont(
		16,
		(m_dataPath / "vgafix.fon").c_str());

	if(!resultFont)
	{
		return Result<Success>{resultFont.moveError()};
	}

	mrefConsole = make_ref<ui::Console>(
		m_refRootElement.get(),
		screenRect,
		ui::BitmapFont::fromFont(
			*resultFont,
			ui::Color(0xFF, 0xFF, 0xFF)));

	return Result{Success{}};
}

void Pi2JammaApplication::render(ui::RenderContext& renderContext)
{
	m_refRootElement->renderTree(renderContext);
}

void Pi2JammaApplication::keyDownEvent(const ui::KeyDownEvent& keyDownEvent)
{
	if(keyDownEvent.getKey() == ui::Key::Escape)
	{
		if (!m_uptScreenControllerStack->pop())
		{
			quit();
		}
	}

	ui::InputEvent inputEvent;

	const KeyConfiguration& keyConfig = mConfiguration.getKeys();

	if (keyDownEvent.getKey() == keyConfig.getUp()) {
		inputEvent.setAction(ui::Action::Up);
	} 
	else if (keyDownEvent.getKey() == keyConfig.getDown()) {
		inputEvent.setAction(ui::Action::Down);
	}
	else if(keyDownEvent.getKey() == keyConfig.getSelect()) {
		inputEvent.setAction(ui::Action::Select);
	}

	m_refRootElement->inputEvent(inputEvent);
}

