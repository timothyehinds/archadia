#pragma once

#include "ui/device/sdl2/sdl2types.hpp"

namespace ui { namespace device { namespace sdl2 {

class Color
{
public:

	using ElementType = decltype(SDL_Color::r);

	Color(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 0x00);
	Color(const std::array<uint8_t,4>& components);
	ElementType getRed() const;
	ElementType getGreen() const;
	ElementType getBlue() const;
	ElementType getAlpha() const;

	void setRed(ElementType red);
	void setGreen(ElementType green);
	void setBlue(ElementType blue);
	void setAlpha(ElementType alpha);

private:
	SDL_Color mSdlColor;

	friend class Application;
};

inline Color::Color(
	ElementType red,
	ElementType green,
	ElementType blue,
	ElementType alpha)
	: mSdlColor({red,green,blue,alpha})
{	
}

inline Color::Color(const std::array<uint8_t,4>& components)
{
	mSdlColor.r = components[0];
	mSdlColor.g = components[1];
	mSdlColor.b = components[2];
	mSdlColor.a = components[3];
}

inline Color::ElementType Color::getRed() const
{
	return mSdlColor.r;

}

inline Color::ElementType Color::getGreen() const
{
	return mSdlColor.g;

}

inline Color::ElementType Color::getBlue() const
{
	return mSdlColor.b;

}

inline Color::ElementType Color::getAlpha() const
{
	return mSdlColor.a;
}

inline void Color::setRed(ElementType red)
{
	mSdlColor.r = red;
}

inline void Color::setGreen(ElementType green)
{
	mSdlColor.g = green;
}

inline void Color::setBlue(ElementType blue)
{
	mSdlColor.b = blue;
}

inline void Color::setAlpha(ElementType alpha)
{
	mSdlColor.a = alpha;
}

}}}

