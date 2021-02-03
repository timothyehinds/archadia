#pragma once

#if 0
#include "Pi2Jamma/games/Games.hpp"
#include "Pi2Jamma/screens/Screen.hpp"
#include "Pi2Jamma/screens/GamesListModel.hpp"

class Pi2JammaApplication;

class GameSelectScreen : public Screen
{
public:
	GameSelectScreen(
		ui::Element* pParent,
		const ui::Rect& rect,
		Pi2JammaApplication& application,
		const Games& games,
		CStr fullThemeDir,
		std::string snapsDir);

	void launchGame(const Game& game);
	const Games& getGames();

	void showSnapForGame(const Game& game);
private:
	
	Pi2JammaApplication& mApplication;
	const Games& mGames;

	std::string mSnapsDir;

	GamesListModel mGamesListModel;
};

inline const Games& GameSelectScreen::getGames()
{
	return mGames;
}

#endif
