#pragma once

#include "ui/device/sdl2/sdl2types.hpp"

namespace ui { namespace device { namespace sdl2 {

class Point
{
public:
	Point(UnitType x = 0, UnitType y = 0);
	
	UnitType getX() const;
	void setX(UnitType x);

	UnitType getY() const;
	void setY(UnitType y);


private:
	SDL_Point mSdlPoint;
};

inline Point::Point(UnitType x, UnitType y)
	: mSdlPoint({x,y})
{
}

inline UnitType Point::getX() const
{
	return mSdlPoint.x;
}

inline void Point::setX(UnitType x)
{
	mSdlPoint.x = x;
}

inline UnitType Point::getY() const
{
	return mSdlPoint.y;
}

inline void Point::setY(UnitType y)
{
	mSdlPoint.y = y;
}

}}}