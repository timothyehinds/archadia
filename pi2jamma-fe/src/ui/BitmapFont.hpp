#pragma once

#include "core/RefCounted.hpp"

#include "ui/Color.hpp"
#include "ui/Font.hpp"
#include "ui/HorizontalAlignment.hpp"
#include "ui/Rect.hpp"
#include "ui/RenderContext.hpp"
#include "ui/Size.hpp"
#include "ui/Surface.hpp"
#include "ui/VerticalAlignment.hpp"

#include <array>

namespace ui {

class BitmapFont : public RefCounted
{
public:
	using GlyphArray = std::array<ref<Surface>, 256>;

	static ref<BitmapFont> fromFont(
		const ref<Font>& refFont,
		const Color& color);

	BitmapFont(GlyphArray glyphArray, UnitType glyphHeight);

	UnitType getHeight() const;
	UnitType getTextWidth(StringSpan);

	void render(
		RenderContext&,
		StringSpan,
		const Rect&,
		HorizontalAlignment,
		VerticalAlignment);

private:

	GlyphArray m_glyphArray;
	UnitType m_glyphHeight;
};

inline UnitType BitmapFont::getHeight() const
{
	return m_glyphHeight;
}

}