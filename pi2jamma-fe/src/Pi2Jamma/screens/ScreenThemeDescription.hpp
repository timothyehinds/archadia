#pragma once

#include "core/RefCounted.hpp"
#include "core/Property.hpp"
#include "ui/HorizontalAlignment.hpp"
#include "ui/Rect.hpp"
#include "ui/Color.hpp"

class ScreenThemeDescription: public RefCounted
{
public:

	static void initialize();

	PROPERTY(ui::Rect, TitleRect);
	PROPERTY(ui::Color, TitleTextColor);
	PROPERTY(ui::HorizontalAlignment, TitleAlignment);

	PROPERTY(ui::Rect, SnapsRect);

	PROPERTY(ui::Rect, MenuRect);
	PROPERTY(ui::Color, MenuTextColor);
	PROPERTY(ui::Color, MenuTextHighlightColor);

	PROPERTY(size_t, MenuTextSize);
	PROPERTY(ui::HorizontalAlignment, MenuTextAlignment);

	PROPERTY(std::string, FontFilePath);
};

