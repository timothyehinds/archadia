#include "ui/elements/Image.hpp"
#include "ui/Application.hpp"

namespace ui {

Image::Image(
	Element* pParent,
	const Rect& rect)
	: Element(pParent, rect)
{
}

Image::Image(
	Element* pParent,
	const Rect& rect,
	CStr filePath)
	: Element(pParent, rect)
{
	loadFromFile(filePath);
}

Image::Image(
	Element* pParent,
	const Rect& rect,
	ref<Surface> refSurface)
	: Element(pParent, rect)
	, m_refSurface{std::move(refSurface)}
{
}


void Image::render(RenderContext& renderContext)
{
	if(!m_refSurface)
	{
		return;
	}

	renderContext.draw(
		m_refSurface,
		getRect().getPosition());
}

void Image::loadFromFile(CStr filePath)
{
	if(filePath.empty())
	{
		m_refSurface = nullptr;
		return;
	}

	Application::get().loadSurface(
		m_refSurface,
		filePath).ignore();
}

void Image::setSurface(ref<ui::Surface> refSurface)
{
	m_refSurface = std::move(refSurface);
}

}
