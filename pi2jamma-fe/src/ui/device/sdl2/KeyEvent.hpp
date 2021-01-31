#pragma once

#include "ui/device/sdl2/Key.hpp"

namespace ui { namespace device { namespace sdl2 {

class KeyEvent
{
public:
	KeyEvent(const SDL_Event& event);

	Key getKey() const;

private:
	const SDL_Event &mSdlEvent;
};

inline KeyEvent::KeyEvent(const SDL_Event& event)
	: mSdlEvent(event)
{
}

inline Key KeyEvent::getKey() const
{
	return static_cast<Key>(mSdlEvent.key.keysym.sym);
}

}}}