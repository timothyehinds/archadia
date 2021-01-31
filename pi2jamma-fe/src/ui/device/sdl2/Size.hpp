#pragma once

#include "ui/device/sdl2/sdl2types.hpp"

namespace ui {
namespace device {
namespace sdl2 {

class Size final
{
public:
	Size(UnitType width = 0, UnitType height = 0);
	
	UnitType getWidth() const;
	void setWidth(UnitType w);

	UnitType getHeight() const;
	void setHeight(UnitType h);

	float getAspectRatio() const;

private:
	UnitType mWidth;
	UnitType mHeight;
};

inline Size::Size(UnitType width, UnitType height)
	: mWidth(width)
	, mHeight(height)
{
}

inline UnitType Size::getWidth() const
{
	return mWidth;
}

inline void Size::setWidth(UnitType width)
{
	mWidth = width;
}

inline UnitType Size::getHeight() const
{
	return mHeight;
}

inline void Size::setHeight(UnitType height)
{
	mHeight = height;
}

inline float Size::getAspectRatio() const
{
	return
		static_cast<float>(mWidth) / static_cast<float>(mHeight);
}

}}}