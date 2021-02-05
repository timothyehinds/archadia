#pragma once

#include "core/serialize/Serializer.hpp"
#include "ui/device/sdl2/Color.hpp"

namespace ui {

using Color = device::sdl2::Color;

}

template<>
struct Serializer<ui::Color>
{
	static Result<Success> load(ui::Color& color, ObjectReadStream& readStream);
	static Result<Success> save(const ui::Color& color, ObjectWriteStream& writeStream);	
};
