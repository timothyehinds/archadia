#pragma once

#include "ui/device/sdl2/sdl2types.hpp"
#include "ui/device/sdl2/Size.hpp"
#include "core/RefCounted.hpp"

namespace ui::device::sdl2 {

class Surface : public RefCounted
{
public:
	Surface() = default;
	Surface(std::unique_ptr<SDL_Texture> uptSdlTexture);	

	const Size& getSize() const;

	UnitType getWidth() const;
	UnitType getHeight() const;

	SDL_Texture* getSdlTexture() const;

private:
	Size mSize;
	std::unique_ptr<SDL_Texture> muptSdlTexture;
};

inline const Size& Surface::getSize() const
{
	return mSize;
}

inline UnitType Surface::getWidth() const
{
	return mSize.getWidth();
}

inline UnitType Surface::getHeight() const
{
	return mSize.getHeight();
}

inline SDL_Texture* Surface:: getSdlTexture() const
{
	return muptSdlTexture.get();
}

}

