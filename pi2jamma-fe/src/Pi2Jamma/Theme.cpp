#include "Pi2Jamma/Theme.hpp"

#include "core/meta/Meta.hpp"

void Theme::initialize()
{
	META_CLASS(Theme);
	META_CLASS_FIELD(Theme, TitleRect);
	META_CLASS_FIELD(Theme, TitleTextColor);
	META_CLASS_FIELD(Theme, TitleAlignment);
	META_CLASS_FIELD(Theme, SnapsRect);
	META_CLASS_FIELD(Theme, MenuRect);

	META_CLASS_FIELD(Theme, MenuTextColor);
	META_CLASS_FIELD(Theme, MenuTextHighlightColor);	
	META_CLASS_FIELD(Theme, MenuTextSize);	
	META_CLASS_FIELD(Theme, MenuTextAlignment);

	META_CLASS_FIELD(Theme, FontFilePath);
}
