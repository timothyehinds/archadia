#pragma once

#include "ui/device/sdl2/sdl2types.hpp"

namespace ui { namespace device { namespace sdl2 {

enum class Key {
	DownArrow = SDLK_DOWN,
	UpArrow = SDLK_UP,
	LeftArrow = SDLK_LEFT,
	RightArrow = SDLK_LEFT,
	Escape = SDLK_ESCAPE,
	Space = SDLK_SPACE
};

}}}
