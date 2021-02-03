#pragma once

#if 0
#include "ui/elements/TextList.hpp"

class GameSelectScreen;

class GamesListModel : public ui::TextListModel
{
public:

	GamesListModel(
		GameSelectScreen& gameSelectScreen);

private:
	virtual size_t getNumItems() override;
	virtual CStr getItem(size_t index) override;
	virtual void onHighlighted(size_t newSelection) override;
	virtual void onSelect(size_t newSelection) override;
	
	GameSelectScreen& mGameSelectScreen;
};
#endif

