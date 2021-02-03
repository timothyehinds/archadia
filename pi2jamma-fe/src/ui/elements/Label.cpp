
#include "ui/elements/Label.hpp"

#include "ui/Application.hpp"
#include "ui/Crop.hpp"
#include "ui/Point.hpp"

namespace ui {

Label::Label(
	Element* pParent,
	const Rect& rect,
	ref<BitmapFont> refBitmapFont,
	const Color& color,
	std::string text,
	HorizontalAlignment horizontalAlignment,
	VerticalAlignment verticalAlignment)
	: Element(pParent, rect)
	, m_refBitmapFont(std::move(refBitmapFont))
	, m_color(color)
	, m_text(std::move(text))
	, m_horizontalAlignment(horizontalAlignment)
	, m_verticalAlignment(verticalAlignment)
{
}


void Label::render(RenderContext& renderContext)
{
	m_refBitmapFont->render(
		renderContext,
		m_text.c_str(),
		getRect(),
		m_horizontalAlignment,
		m_verticalAlignment);
}

} // namespaces

