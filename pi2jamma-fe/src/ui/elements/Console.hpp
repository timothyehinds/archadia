#pragma once

#include "ui/BitmapFont.hpp"
#include "ui/elements/Element.hpp"

#include <vector>

namespace ui {

class Console : public Element
{
public:

	Console(
		Element* pParent,
		const Rect& rect,
		ref<BitmapFont> refBitmapFont);

	void clear();

protected:

	virtual void render(RenderContext& renderContext) override;

	ref<BitmapFont> mrefBitmapFont;
	std::vector<std::string> mLines;

};

}