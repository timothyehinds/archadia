#include "ui/elements/Image.hpp"
#include "ui/Application.hpp"

namespace ui {

Image::Image(
	Element* pParent,
	const Rect& rect,
	CStr filePath)
	: Element(pParent, rect)
{
	loadFromFile(filePath);
}

void Image::render(RenderContext& renderContext)
{
	if(mrefSurface.isNull()) {
		return;
	}

	renderContext.draw(
		mrefSurface,
		getRect().getPosition());
}

void Image::loadFromFile(CStr filePath)
{
	if(filePath.empty())
	{
		mrefSurface = nullptr;
		return;
	}

	Application::get().loadSurface(
		mrefSurface,
		filePath).ignore();
}

void Image::setSurface(ref<ui::Surface> refSurface)
{
	mrefSurface = std::move(refSurface);
}

}
