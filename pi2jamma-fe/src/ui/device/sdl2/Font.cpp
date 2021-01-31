#include "ui/device/sdl2/Font.hpp"

namespace ui { namespace device { namespace sdl2 {

Font::Font(std::unique_ptr<TTF_Font> uptSdlFont)
	: muptSdlFont(std::move(uptSdlFont))
{
}

}}}