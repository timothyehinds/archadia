#include "Pi2Jamma/Configuration/KeyConfiguration.hpp"

#include "core/meta/Meta.hpp"

void KeyConfiguration::initialize()
{
	META_CLASS(KeyConfiguration);
	META_CLASS_PROPERTY(KeyConfiguration, Up);
	META_CLASS_PROPERTY(KeyConfiguration, Down);	
	META_CLASS_PROPERTY(KeyConfiguration, Left);
	META_CLASS_PROPERTY(KeyConfiguration, Right);
	META_CLASS_PROPERTY(KeyConfiguration, Select);
	META_CLASS_PROPERTY(KeyConfiguration, Back);
}

KeyConfiguration::KeyConfiguration()
	: mUp(ui::Key::UpArrow)
	, mDown(ui::Key::DownArrow)
	, mLeft(ui::Key::LeftArrow)
	, mRight(ui::Key::RightArrow)
	, mSelect(ui::Key::Space)
	, mBack(ui::Key::Escape)
{}
