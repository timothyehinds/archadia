#pragma once

#include "core/Property.hpp"

#include "ui/Orientation.hpp"

#include <string>
class UiConfiguration
{
public:

	static void initialize();

	PROPERTY(ui::Orientation, ScreenOrientation);
	PROPERTY(std::string, PortraitTheme);
	PROPERTY(std::string, LandscapeTheme);	
};