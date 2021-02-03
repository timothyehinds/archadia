#include "Pi2Jamma/Pi2JammaApplication.hpp"

#include "Pi2Jamma/CommandLine/CommandLineHandlerConfigFile.hpp"
#include "Pi2Jamma/screens/ScreenTheme.hpp"
#include "Pi2Jamma/screens/SettingsScreenController.hpp"

#include "core/file/FilePath.hpp"
#include "core/meta/Meta.hpp"
#include "core/json/JsonParser.hpp"
#include "core/serialize/json/JsonSerialize.hpp"
#include "core/CommandLine/CommandLine.hpp"
#include "ui/ui.hpp"
#include "ui/Point.hpp"

Pi2JammaApplication::Pi2JammaApplication()
	: mDataDir("./data")
{
}

Result Pi2JammaApplication::initialize(int argc, const char* argv[])
{
	Meta::initialize();
	ui::initialize();

	Configuration::initialize();
	ScreenThemeDescription::initialize();
	//Game::initialize();
	//Games::initialize();

	Result result = CommandLine::get().parse(argc, argv);
	if(result.peekFailed()) {
		return result;
	}

	result = loadConfiguration();
	if(result.peekFailed()) {
		return result;
	}

	return setupUi();
}

Result Pi2JammaApplication::loadConfiguration()
{
	Result result = loadJson(
		mConfiguration,
		CommandLineHandlerConfigFile::sSingleton.mConfigFile);

	if(result.peekFailed()) {
		return result;
	}

	std::string themesDir = joinPath(mDataDir, "themes");
	const UiConfiguration& uiConfig = mConfiguration.getUi();

	bool portrait =
		(ui::Orientation::Portrait == uiConfig.getScreenOrientation());

	CStr orientationDir =
		portrait
			? "portrait"
			: "landsape";

	CStr themeDir =
		portrait
			? uiConfig.getPortraitTheme()
			: uiConfig.getLandscapeTheme();

	mSnapsDir = joinPath(mDataDir, "snaps", orientationDir);			

	// Load theme

	m_fullThemeDir =
		joinPath(themesDir, orientationDir, themeDir);

#if 0
	// Load Games

	std::string gamesPath = joinPath(mDataDir, "games.txt");

	result = loadJson(mGames, gamesPath);
	if(result.peekFailed()) {
		return result;
	}
	#endif



	return Result::makeSuccess();
}
Result Pi2JammaApplication::setupUi()
{
	ui::Rect screenRect(
		ui::Point(0, 0),
		getScreenSize());

	m_refRootElement =
		make_ref<ui::Element>(
			nullptr,
			screenRect);

	loadJson(m_screenThemeDescription, joinPath(m_fullThemeDir, "config.txt")).catastrophic();

	ref<ScreenTheme> refScreenTheme{
		make_ref<ScreenTheme>(
			m_screenThemeDescription,
			m_fullThemeDir.c_str())};

	std::unique_ptr<SettingsScreenController> uptSettingsScreenController{
		std::make_unique<SettingsScreenController>(refScreenTheme)};

	m_screenControllerStack.push(
		m_refRootElement.get(),
		m_refRootElement->getRect(),
		std::move(uptSettingsScreenController));

	/*mrefGameSelectScreen =
		make_ref<GameSelectScreen>(
			m_refRootElement.get(),
			screenRect,
			*this,
			mGames,
			m_fullThemeDir,
			mSnapsDir); */

		

	ref<ui::Font> refConsoleFont;
	Result r = loadFont(
		refConsoleFont,
		16,
		joinPath(mDataDir, "vgafix.fon"));

	if(r.peekFailed()) {
		return r;
	}

	mrefConsole = make_ref<ui::Console>(
		m_refRootElement.get(),
		screenRect,
		ui::BitmapFont::fromFont(
			refConsoleFont,
			ui::Color(0xFF, 0xFF, 0xFF)));

	return Result::makeSuccess();
}

void Pi2JammaApplication::render(ui::RenderContext& renderContext)
{
	m_refRootElement->renderTree(renderContext);
}

void Pi2JammaApplication::keyDownEvent(const ui::KeyDownEvent& keyDownEvent)
{

	if(keyDownEvent.getKey() == ui::Key::Escape)
	{
		quit();
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
