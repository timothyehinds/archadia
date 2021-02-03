#include "ui/elements/Console.hpp"

namespace ui {

Console::Console(
	Element* pParent,
	const Rect& rect,
	ref<BitmapFont> refBitmapFont)
	: Element(pParent, rect)
	, mrefBitmapFont(std::move(refBitmapFont))
{
	mLines.push_back("Hello.");
	mLines.push_back("World.");
}

void Console::clear()
{
	mLines.clear();
}

void Console::render(RenderContext& renderContext)
{
	auto fontHeight = mrefBitmapFont->getHeight();

	Rect rect(0, 0, getWidth(), fontHeight);

	for(auto&& line : mLines)
	{
		mrefBitmapFont->render(
			renderContext,
			line,
			rect,
			HorizontalAlignment::Left,
			VerticalAlignment::Center);

		rect.setY(rect.getY() + fontHeight);
	}
}

}
