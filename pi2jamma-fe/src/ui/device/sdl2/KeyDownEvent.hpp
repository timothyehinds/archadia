#pragma once

#include "ui/device/sdl2/KeyEvent.hpp"

namespace ui { namespace device { namespace sdl2 {

class KeyDownEvent final : public KeyEvent
{
public:
	KeyDownEvent(const SDL_Event& event);
};

inline KeyDownEvent::KeyDownEvent(const SDL_Event& event)
	: KeyEvent(event)
{
}

}}}
