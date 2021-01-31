#include "ui/elements/Element.hpp"
#include "ui/Color.hpp"

namespace ui {

Element::Element(Element* pParent, const Rect& rect)
	: mpParent(pParent)
	, mRect(rect)
{
	if(mpParent) {
		mpParent->mChildren.insertTail(*this);
	}
}

void Element::setRect(const Rect& rect)
{
	auto oldSize = mRect.getSize();

	mRect = rect;
	
	resize(oldSize, mRect.getSize());
}

Element::~Element()
{
	if(mpParent) {
		mpParent->mChildren.remove(*this);
	}
}

void Element::renderTree(RenderContext& renderContext)
{
	render(renderContext);

	#ifdef DEBUG_ELEMENT
		/* renderContext.drawRect(
			getRect(),
			ui::Color(0x80, 0x80, 0x80, 0x80) ); */
	#endif

	
	for(auto && child : mChildren) {
		child.renderTree(renderContext);
	}
}

void Element::inputEvent(InputEvent& inputEvent)
{
	input(inputEvent);
	
	for(auto&& child : mChildren) {
		child.inputEvent(inputEvent);
	}
}

}
