#pragma once

#include "core/serialize/Serializer.hpp"
#include "ui/device/sdl2/Color.hpp"

namespace ui {

using Color = device::sdl2::Color;

}

template<>
struct Serializer<ui::Color>
{
	static Result load(ui::Color& color, ObjectReadStream& readStream);
	static Result save(const ui::Color& color, ObjectWriteStream& writeStream);	
};
