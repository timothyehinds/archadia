#include "ui/device/sdl2/Surface.hpp"

#include "debug.hpp"

namespace ui {
namespace device {
namespace sdl2 {

Surface::Surface(std::unique_ptr<SDL_Texture> uptSdlTexture)
	: muptSdlTexture(std::move(uptSdlTexture))
{
	UnitType width;
	UnitType height;

	int result =
		SDL_QueryTexture(
			muptSdlTexture.get(),
			nullptr,
			nullptr,
			& width,
			& height);

	mSize = Size(width, height);

	ASSERT(0 == result);
}
}}}
