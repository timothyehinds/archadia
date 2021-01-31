#include "Pi2Jamma/games/Games.hpp"

#include "core/meta/Meta.hpp"
#include "core/serialize/SerializeVector.hpp"

void Game::initialize()
{
	META_CLASS(Game);
	META_CLASS_PROPERTY(Game, DisplayName);
	META_CLASS_PROPERTY(Game, RomName);
	META_CLASS_PROPERTY(Game, SnapName);
	META_CLASS_PROPERTY(Game, Orientation);
}

void Games::initialize()
{
	using GameList = std::vector<Game>;
	META_PRIMITIVE(GameList);

	META_CLASS(Games);
	META_CLASS_PROPERTY(Games, GameList);
}