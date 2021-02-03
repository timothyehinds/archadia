#pragma once

#include "ui/elements/Element.hpp"
#include "ui/Color.hpp"
#include "ui/BitmapFont.hpp"
#include "ui/HorizontalAlignment.hpp"
#include "ui/VerticalAlignment.hpp"
#include "ui/Surface.hpp"

namespace ui {

class Label final : public Element
{
public:

	Label(
		Element* pParent,
		const Rect& rect,
		ref<BitmapFont> refBitmapFont,
		const Color& color,
		std::string text,
		HorizontalAlignment horizontalAlignment
			= HorizontalAlignment::Left,
		VerticalAlignment verticalAlignment
			= VerticalAlignment::Center);		

protected:

	virtual void render(RenderContext& renderContext) override;
	
private:

	ref<Surface> m_refSurface;
	ref<BitmapFont> m_refBitmapFont;
	Color m_color;
	std::string m_text;
	HorizontalAlignment m_horizontalAlignment;
	VerticalAlignment m_verticalAlignment;
};

}
