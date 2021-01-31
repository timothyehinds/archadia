
#include "ui/elements/Label.hpp"

#include "ui/Application.hpp"
#include "ui/Crop.hpp"
#include "ui/Point.hpp"

namespace ui {

Label::Label(
	Element* pParent,
	const Rect& rect,
	ref<Font> refFont,
	const Color& color,
	std::string text,
	HorizontalAlignment horizontalAlignment,
	VerticalAlignment verticalAlignment)
	: Element(pParent, rect)
	, mrefFont(std::move(refFont))
	, mColor(color)
	, mText(std::move(text))
	, mHorizontalAlignment(horizontalAlignment)
	, mVerticalAlignment(verticalAlignment)

{
	Result r =
		Application::get().renderText(
			mrefSurface,
			mrefFont,
			mColor,
			mText.c_str());

	r.ignore();
}


void Label::render(RenderContext& renderContext)
{
	auto fitRects =
		fitRect(
			Rect(
				Point(0, 0),
				mrefSurface->getSize()),
			getRect(),
			CropMode::None,
			HorizontalAlignment::Left,
			VerticalAlignment::Center,
			mHorizontalAlignment,
			mVerticalAlignment);

	renderContext.draw(
		mrefSurface,
		fitRects.getTargetRect(),
		fitRects.getSourceRect());
}

}

