#include "Pi2Jamma/Pi2JammaApplication.hpp"

#include "Pi2Jamma/CommandLine/CommandLineHandlerConfigFile.hpp"
#include "Pi2Jamma/Theme.hpp"
#include "Pi2Jamma/screens/GameSelectScreen.hpp"

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
	Theme::initialize();
	Game::initialize();
	Games::initialize();

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

	mFullThemeDir =
		joinPath(themesDir, orientationDir, themeDir);

	// Load Games

	std::string gamesPath = joinPath(mDataDir, "games.txt");

	result = loadJson(mGames, gamesPath);
	if(result.peekFailed()) {
		return result;
	}

	return Result::makeSuccess();
}
Result Pi2JammaApplication::setupUi()
{
	ui::Rect screenRect(
		ui::Point(0, 0),
		getScreenSize());

	mrefRootElement =
		make_ref<ui::Element>(
			nullptr,
			screenRect);

	mrefGameSelectScreen =
		make_ref<GameSelectScreen>(
			mrefRootElement.get(),
			screenRect,
			*this,
			mGames,
			mFullThemeDir,
			mSnapsDir);

	ref<ui::Font> refConsoleFont;
	Result r = loadFont(
		refConsoleFont,
		16,
		joinPath(mDataDir, "vgafix.fon"));

	if(r.peekFailed()) {
		return r;
	}

	mrefConsole = make_ref<ui::Console>(
		mrefRootElement.get(),
		screenRect,
		ui::BitmapFont::fromFont(
			refConsoleFont,
			ui::Color(0xFF, 0xFF, 0xFF)));

	return Result::makeSuccess();
}

void Pi2JammaApplication::render(ui::RenderContext& renderContext)
{
	mrefRootElement->renderTree(renderContext);
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

	mrefRootElement->inputEvent(inputEvent);
}
