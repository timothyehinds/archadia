#include "Pi2Jamma/screens/GameSelectScreen.hpp"

#if 0

#include "Pi2Jamma/Pi2JammaApplication.hpp"

#include "ui/Point.hpp"
#include "core/file/FilePath.hpp"
#include "core/serialize/json/JsonSerialize.hpp"

GameSelectScreen::GameSelectScreen(
	ui::Element* pParent,
	const ui::Rect& rect,
	Pi2JammaApplication& application,
	const Games& games,
	CStr fullThemeDir,
	std::string snapsDir)
	: Screen(
		pParent,
		rect)
	, mApplication(application)
	, mGames(games)
	, mSnapsDir(std::move(snapsDir))
	, mGamesListModel(*this)
{

}

void GameSelectScreen::launchGame(const Game& game)
{
	LogFmt("Select game!: %s\n", game.getDisplayName().c_str());
}

void GameSelectScreen::showSnapForGame(const Game& game)
{
	auto&& snapName = game.getSnapName();
	if(snapName.size() <= 0)
	{
		mrefSnapsImage->setSurface(nullptr);
		return;
	}

	mrefSnapsImage->loadFromFile(
		joinPath(
			mSnapsDir,
			game.getSnapName()));
}

#endif

