#pragma once

#include "core/Property.hpp"
#include "ui/Key.hpp"

class KeyConfiguration
{
	public:

		static void initialize();

		KeyConfiguration();
		
		PROPERTY(ui::Key, Up)
		PROPERTY(ui::Key, Down);
		PROPERTY(ui::Key, Left);
		PROPERTY(ui::Key, Right);
		PROPERTY(ui::Key, Select);
		PROPERTY(ui::Key, Back);
};
