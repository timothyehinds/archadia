#include "Pi2Jamma/Configuration/Configuration.hpp"

#include "core/meta/Meta.hpp"

void Configuration::initialize()
{
	KeyConfiguration::initialize();
	UiConfiguration::initialize();

	META_CLASS(Configuration);
	META_CLASS_PROPERTY(Configuration, SaveConfigOnExit);
	META_CLASS_PROPERTY(Configuration, Keys);
	META_CLASS_PROPERTY(Configuration, Ui);
}


Configuration::Configuration()
	: mSaveConfigOnExit(true)
{}