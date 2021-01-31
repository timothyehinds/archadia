#pragma once

#include "core/Property.hpp"

#include "ui/elements/Action.hpp"

namespace ui {

class InputEvent
{
public:
	InputEvent(Action action = Action::None);

	PROPERTY(Action, Action);
};

inline InputEvent::InputEvent(Action action)
	: mAction(action)
{	
}

}