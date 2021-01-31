#include "ui/device/sdl2/Application.hpp"

namespace ui { namespace device { namespace sdl2 {

Application* Application::spSingleton = nullptr;

Application::Application()
	: mQuit(false)
{
	ASSERT(nullptr == spSingleton);
	spSingleton = this;
}

Application::~Application()
{
	muptSdlRenderer.release();
	muptSdlWindow.release();

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	spSingleton = nullptr;
}

Result Application::run(int argc, const char* argv[])
{
	int sdlInitResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	if(0 != sdlInitResult) {
		return Result::makeFailureWithString(SDL_GetError());
	}

	int imgInitFlags = IMG_INIT_PNG;
	int imgInitResult = IMG_Init(imgInitFlags);

	if(imgInitFlags != imgInitResult) {
		return Result::makeFailureWithString(IMG_GetError());
	}

	int ttfInitResult = TTF_Init();
	if(0 != ttfInitResult) {
		return Result::makeFailureWithString(TTF_GetError());
	}

	muptSdlWindow.reset(
		SDL_CreateWindow(
			"My Window",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			240,
			320,
			SDL_WINDOW_SHOWN));

	if(!muptSdlWindow) {
		return Result::makeFailureWithString(SDL_GetError());
	}

	muptSdlRenderer.reset(
			SDL_CreateRenderer(
				muptSdlWindow.get(),
				-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

	if(!muptSdlRenderer) {
		return Result::makeFailureWithString(SDL_GetError());
	}

	Result result = initialize(argc, argv);
	if (result.peekFailed()) {
		return result;
	}

	return renderLoop();
}

Result Application::renderLoop()
{
	SDL_Event sdlEvent = {};

	while(!mQuit)
	{
		if( 0 != SDL_PollEvent(&sdlEvent))
		{
			dispatchEvent(sdlEvent);
		}

		SDL_RenderClear(muptSdlRenderer.get());
		RenderContext renderContext(*muptSdlRenderer);
		render(renderContext);
		SDL_RenderPresent(muptSdlRenderer.get());
		SDL_Delay(60);
	}

	return Result::makeSuccess();
}

Result Application::loadSurface(
	ref<Surface>& refSurface,
	CStr filePath)
{
	std::unique_ptr<SDL_Texture> uptSdlTexture(
		IMG_LoadTexture(
			muptSdlRenderer.get(),
			filePath.c_str()));

	if(!uptSdlTexture) {
		LogFmt( "Failed to load texture: %s\n", filePath.c_str());		
		return Result::makeFailureWithString(IMG_GetError());
	}

	refSurface = make_ref<Surface>(std::move(uptSdlTexture));

	return Result::makeSuccess();
}

Result Application::loadFont(
	ref<Font>& refFont,
	UnitType sizePx,
	CStr fileName)
{
	std::unique_ptr<TTF_Font> uptSdlFont(
		TTF_OpenFont(fileName.c_str(), sizePx));

	if(!uptSdlFont) {
		return Result::makeFailureWithString(TTF_GetError());
	}

	refFont = make_ref<Font>(std::move(uptSdlFont));

	return Result::makeSuccess();
}

Result Application::renderText(
	ref<Surface>& refSurface,
	const ref<Font>& refFont,
	const Color& color,
	CStr text)
{
	std::unique_ptr<SDL_Surface> uptSdlSurface(
		TTF_RenderText_Solid(
			refFont->muptSdlFont.get(),
			text.c_str(),
			color.mSdlColor));

	if(!uptSdlSurface) {
		return Result::makeFailureWithString(TTF_GetError());
	}

	std::unique_ptr<SDL_Texture> uptSdlTexture(
		SDL_CreateTextureFromSurface(
			muptSdlRenderer.get(),
			uptSdlSurface.get()));

	if(!uptSdlTexture) {
		return Result::makeFailureWithString(SDL_GetError());
	}

	refSurface = make_ref<Surface>(std::move(uptSdlTexture));

	return Result::makeSuccess();
}

void Application::dispatchEvent(const SDL_Event& sdlEvent)
{
	if(SDL_QUIT == sdlEvent.type)
	{
		quit();
	}
	else if (SDL_KEYDOWN == sdlEvent.type)
	{
		keyDownEvent(KeyDownEvent(sdlEvent));
	}
}

Size Application::getScreenSize() {
	int w;
	int h;
	SDL_GetWindowSize(
		muptSdlWindow.get(),
		&w, 
		&h);
	return Size(w,h);
}

}}}