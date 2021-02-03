#include "Pi2Jamma/screens/ScreenThemeDescription.hpp"

#include "core/meta/Meta.hpp"

void ScreenThemeDescription::initialize()
{
	META_CLASS(ScreenThemeDescription);
	META_CLASS_FIELD(ScreenThemeDescription, TitleRect);
	META_CLASS_FIELD(ScreenThemeDescription, TitleTextColor);
	META_CLASS_FIELD(ScreenThemeDescription, TitleAlignment);
	META_CLASS_FIELD(ScreenThemeDescription, SnapsRect);
	META_CLASS_FIELD(ScreenThemeDescription, MenuRect);

	META_CLASS_FIELD(ScreenThemeDescription, MenuTextColor);
	META_CLASS_FIELD(ScreenThemeDescription, MenuTextHighlightColor);	
	META_CLASS_FIELD(ScreenThemeDescription, MenuTextSize);	
	META_CLASS_FIELD(ScreenThemeDescription, MenuTextAlignment);

	META_CLASS_FIELD(ScreenThemeDescription, FontFilePath);
}
