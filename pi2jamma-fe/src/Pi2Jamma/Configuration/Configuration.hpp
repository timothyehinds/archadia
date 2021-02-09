#pragma once

#include "Pi2Jamma/Configuration/KeyConfiguration.hpp"
#include "Pi2Jamma/Configuration/UiConfiguration.hpp"

#include "ui/Orientation.hpp"

class Configuration
{
public:
	static void initialize();

	Configuration();

	PROPERTY(bool, SaveConfigOnExit);
	PROPERTY(KeyConfiguration, Keys);
	PROPERTY(UiConfiguration, Ui);

};