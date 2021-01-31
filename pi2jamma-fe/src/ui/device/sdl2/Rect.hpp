#pragma once

#include "ui/device/sdl2/Point.hpp"
#include "ui/device/sdl2/Size.hpp"

namespace ui {
namespace device {
namespace sdl2 {

class Rect
{
public:
	Rect();
	Rect(UnitType x, UnitType y, UnitType w, UnitType h);
	Rect(const Point& point, const Size& size);

	UnitType getX() const;
	void setX(UnitType x);

	UnitType getY() const;
	void setY(UnitType y);

	UnitType getWidth() const;
	void setWidth(UnitType width);

	UnitType getHeight() const;
	void setHeight(UnitType height);

	UnitType getYCenter() const;

	Size getSize() const;
	void setSize(Size size);

	Point getPosition() const;
	void setPosition(const Point& position);

	UnitType getLeft() const;
	UnitType getTop() const;

	UnitType getRightExclusive() const;
	UnitType getBottomExclusive() const;

	float getAspectRatio() const;

	const SDL_Rect& getSdl2Rect() const;
	
private:
	SDL_Rect mSdlRect;

};

inline Rect::Rect(UnitType x, UnitType y, UnitType w, UnitType h)
	: mSdlRect({x,y,w,h})
{
}

inline Rect::Rect(const Point& point, const Size& size)
	: mSdlRect({
		point.getX(),
		point.getY(),
		size.getWidth(),
		size.getHeight()})
{

}

inline Rect::Rect()
	: mSdlRect({0,0,0,0})
{
}

inline UnitType Rect::getX() const
{
	return mSdlRect.x;
}

inline void Rect::setX(UnitType x) 
{
	mSdlRect.x = x;
}

inline UnitType Rect::getY() const
{
	return mSdlRect.y;
}

inline void Rect::setY(UnitType y)
{
	mSdlRect.y = y;
}

inline UnitType Rect::getWidth() const
{
	return mSdlRect.w;
}

inline void Rect::setWidth(UnitType w) 
{
	mSdlRect.w = w;
}

inline UnitType Rect::getHeight() const
{
	return mSdlRect.h;
}

inline void Rect::setHeight(UnitType height)
{
	mSdlRect.h = height;
}

inline UnitType Rect::getYCenter() const
{
	return mSdlRect.y + (mSdlRect.h / 2);
}

inline Point Rect::getPosition() const
{
	return Point(getX(), getY());
}

inline void Rect::setPosition(const Point& position)
{
	mSdlRect.x = position.getX();
	mSdlRect.y = position.getY();
}

inline UnitType Rect::getLeft() const
{
	return mSdlRect.x;
}

inline UnitType Rect::getTop() const
{
	return mSdlRect.y;
}

inline UnitType Rect::getRightExclusive() const
{
	return mSdlRect.x + mSdlRect.w;
}

inline UnitType Rect::getBottomExclusive() const
{
	return mSdlRect.y + mSdlRect.h;
}

inline float Rect::getAspectRatio() const
{
	return getSize().getAspectRatio();
}

inline const SDL_Rect& Rect::getSdl2Rect() const
{
	return mSdlRect;
}

inline Size Rect::getSize() const
{
	return Size(mSdlRect.w, mSdlRect.h);
}

inline void Rect::setSize(Size size)
{
	mSdlRect.w = size.getWidth();
	mSdlRect.h = size.getHeight();
}

}}}
