#pragma once

#include "core/Property.hpp"

#include "ui/Orientation.hpp"
#include <string>
#include <vector>

class Game
{
public:
	static void initialize();

	PROPERTY(std::string, DisplayName);
	PROPERTY(std::string, SnapName);
	PROPERTY(std::string, RomName);
	PROPERTY(ui::Orientation, Orientation);
};

class Games
{
public:
	static void initialize();

	PROPERTY(std::vector<Game>, GameList);
};