#include "ui/BitmapFont.hpp"

#include "ui/Application.hpp"
#include "ui/Point.hpp"

namespace ui {

ref<BitmapFont> BitmapFont::fromFont(
	const ref<Font>& refFont,
	const Color& color)
{
	char c[2];
	c[1] = '\0';

	GlyphArray glyphArray;
	UnitType glyphHeight(0);

	for(size_t i = 1; i < 256; i ++ )
	{
		c[0] = static_cast<char>(i);

		ref<Surface>& refSurface = glyphArray[i];

		Result r =
			Application::get().renderText(
				refSurface,
				refFont,
				color,
				c);

		if (refSurface)
		{
			glyphHeight = std::max(glyphHeight, refSurface->getHeight());
		}
	}

	return make_ref<BitmapFont>(std::move(glyphArray), glyphHeight);
}

BitmapFont::BitmapFont(
	GlyphArray glyphArray,
	UnitType glyphHeight)
	: m_glyphArray(std::move(glyphArray))
	, m_glyphHeight(glyphHeight)
{
}

void BitmapFont::render(
	RenderContext& renderContext,
	const StringSpan stringSpan,
	const Rect& rect,
	const HorizontalAlignment horizontalAlignment,
	const VerticalAlignment verticalAlignment)
{
	UnitType xOffset{0};

	if (horizontalAlignment == HorizontalAlignment::Center)
	{
		UnitType textWidth = getTextWidth(stringSpan);

		xOffset = (rect.getWidth() - textWidth) / 2;
	}

	Point point(rect.getX() + xOffset, rect.getY());

	for(auto&& c : stringSpan)
	{
		ref<Surface>& refSurface = m_glyphArray[c];

		if(!refSurface)
		{
			continue;
		}

		renderContext.draw(
			refSurface,
			point);

		point.setX(point.getX() + refSurface->getWidth());

		if(point.getX() > rect.getRightExclusive()) {
			break;
		}
	}
}

UnitType BitmapFont::getTextWidth(const StringSpan stringSpan)
{
	UnitType width{0};

	for(const char c : stringSpan)
	{
		if (const ref<Surface> refSurface{m_glyphArray[c]})
		{
			width += refSurface->getWidth();
		}
	}

	return width;
}

} // namespaces