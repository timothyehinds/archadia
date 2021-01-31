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

	for(size_t i = 0; i < 256; i ++ )
	{
		c[0] = static_cast<char>(i);

		ref<Surface>& refSurface = glyphArray[i];

		Result r =
			Application::get().renderText(
				refSurface,
				refFont,
				color,
				c);

		if (refSurface.isValid()) {
			glyphHeight = std::max(glyphHeight, refSurface->getHeight());
		}

		r.ignore();
	}

	return make_ref<BitmapFont>(std::move(glyphArray), glyphHeight);
}

BitmapFont::BitmapFont(
	GlyphArray glyphArray,
	UnitType glyphHeight)
	: mGlyphArray(std::move(glyphArray))
	, mGlyphHeight(glyphHeight)
{
}

void BitmapFont::render(
	RenderContext& renderContext,
	StringSpan stringSpan,
	const Rect& rect)
{
	Point point(rect.getX(), rect.getY());

	for(auto&& c : stringSpan)
	{
		ref<Surface>& refSurface = mGlyphArray[c];
		if(refSurface.isNull()) {
	
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

}