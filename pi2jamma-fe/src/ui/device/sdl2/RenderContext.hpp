#pragma once

#include "ui/device/sdl2/Color.hpp"
#include "ui/device/sdl2/Point.hpp"
#include "ui/device/sdl2/Surface.hpp"
#include "ui/device/sdl2/Rect.hpp"

namespace ui::device::sdl2 {

class Application;

class RenderContext
{
	public:
		RenderContext(SDL_Renderer& sdl2Renderer);

		void draw(const ref<Surface>& refSurface, const Rect& target, const Rect& source);
		void draw(const ref<Surface>& refSurface, const Point& targetPoint);
		void drawRect(const Rect& rect, const Color& color);

	private:
		SDL_Renderer& mSdl2Renderer;
};

}