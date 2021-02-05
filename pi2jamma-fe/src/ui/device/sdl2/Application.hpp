#pragma once

#include "core/CString.hpp"
#include "core/Result.hpp"

#include "ui/device/sdl2/sdl2types.hpp"
#include "ui/device/sdl2/Color.hpp"
#include "ui/device/sdl2/Rect.hpp"
#include "ui/device/sdl2/RenderContext.hpp"
#include "ui/device/sdl2/KeyDownEvent.hpp"
#include "ui/device/sdl2/Surface.hpp"
#include "ui/device/sdl2/Font.hpp"

namespace ui::device::sdl2 {

class Application
{
public:

	Application();
	~Application();

	static Application& get();

	Result<Success> run(int argc, const char* argv[]);
	void quit();

	virtual Result<Success> initialize(int argc, const char* argv[]) = 0;
	virtual void render(RenderContext& renderContext) = 0;
	virtual void keyDownEvent(const KeyDownEvent& keyDownEvent) = 0;

	Result<ref<Surface>> loadSurface(CStr fileName);
	
	Result<ref<Font>> loadFont(
		UnitType sizePx,
		CStr fileName);
	
	Result<Success> renderText(
		ref<Surface>& refSurface,
		const ref<Font>& refFont,
		const Color& color,
		CStr text);

	Size getScreenSize();

private:

	Result<Success> renderLoop();
	void dispatchEvent(const SDL_Event& sdlEvent);		

	bool mQuit;

	std::unique_ptr<SDL_Renderer> muptSdlRenderer;
	std::unique_ptr<SDL_Window> muptSdlWindow;
	std::unique_ptr<SDL_Texture> m_uptUnRotatedTexture;
	std::unique_ptr<SDL_Texture> m_uptRotatedTexture;

	ref<Surface> mrefBackBuffer;

	static Application* spSingleton;

};

inline Application& Application::get()
{
	ASSERT(nullptr != spSingleton);
	return *spSingleton;
}

inline void Application::quit()
{
	mQuit = true;
}

}
