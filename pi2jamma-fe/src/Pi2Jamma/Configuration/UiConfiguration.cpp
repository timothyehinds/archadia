#include "Pi2Jamma/Configuration/UiConfiguration.hpp"

#include "core/meta/Meta.hpp"

void UiConfiguration::initialize()
{
	META_CLASS(UiConfiguration);
	META_CLASS_PROPERTY(UiConfiguration, ScreenOrientation);
	META_CLASS_PROPERTY(UiConfiguration, PortraitTheme);
	META_CLASS_PROPERTY(UiConfiguration, LandscapeTheme);
}