#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <memory>

namespace std
{
	template<>
	struct default_delete<SDL_Renderer>
	{
		void operator()(SDL_Renderer* pSdlRenderer) {
			SDL_DestroyRenderer(pSdlRenderer);
		}
	};

	template<>
	struct default_delete<SDL_Window>
	{
		void operator()(SDL_Window* pSdlWindow) {
			SDL_DestroyWindow(pSdlWindow);
		}
	};

	template<>
	struct default_delete<SDL_Texture>
	{
		void operator()(SDL_Texture* pSdlTexture) {
			SDL_DestroyTexture(pSdlTexture);
		}
	};

	template<>
	struct default_delete<TTF_Font>
	{
		void operator()(TTF_Font* pSdlTtf) {
			TTF_CloseFont(pSdlTtf);
		}
	};
}

using UnitType = int;