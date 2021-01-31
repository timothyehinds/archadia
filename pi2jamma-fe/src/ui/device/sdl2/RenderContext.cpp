#include "ui/device/sdl2/RenderContext.hpp"

namespace ui {
namespace device {
namespace sdl2 {

RenderContext::RenderContext(SDL_Renderer& renderer)
	: mSdl2Renderer(renderer)
{
}

void RenderContext::draw(
	const ref<Surface>& refSurface,
	const Rect& target,
	const Rect& source)
{
	SDL_RenderCopy(
		&mSdl2Renderer,
		refSurface->getSdlTexture(),
		& source.getSdl2Rect(),
		& target.getSdl2Rect());	
}


void RenderContext::draw(
	const ref<Surface>& refSurface,
	const Point& targetPoint)
{
	SDL_Rect dst = {
		targetPoint.getX(),
		targetPoint.getY(),
		refSurface->getSize().getWidth(),
		refSurface->getSize().getHeight() };

	SDL_RenderCopy(
		&mSdl2Renderer,
		refSurface->getSdlTexture(),
		nullptr,
		&dst);
}

void RenderContext::drawRect(const Rect& rect, const Color& color)
{
	auto result =
		SDL_SetRenderDrawColor(
			& mSdl2Renderer,
			color.getRed(),
			color.getGreen(),
			color.getBlue(),
			color.getAlpha());

	ASSERT(0 == result);

	result =
		SDL_RenderDrawRect(
			& mSdl2Renderer,
			& rect.getSdl2Rect());

	ASSERT(0 == result);
}

}}}

