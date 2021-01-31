#pragma once

#include "ui/BitmapFont.hpp"
#include "ui/elements/Label.hpp"
#include "ui/elements/Image.hpp"
#include "ui/elements/TextList.hpp"

#include "Pi2Jamma/screens/GamesListModel.hpp"
#include "Pi2Jamma/Theme.hpp"
#include "Pi2Jamma/games/Games.hpp"


class Pi2JammaApplication;

class GameSelectScreen : public ui::Element
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
	Theme mTheme;
	std::string mSnapsDir;

	GamesListModel mGamesListModel;

	ref<ui::BitmapFont> mrefSelectedFont;
	ref<ui::BitmapFont> mrefUnselectedFont;

	ref<ui::Image> mrefBackgroundImage;
	ref<ui::Label> mrefTitle;
	ref<ui::TextList> mrefTextList;
	ref<ui::Image> mrefSnapsImage;
};

inline const Games& GameSelectScreen::getGames()
{
	return mGames;
}
