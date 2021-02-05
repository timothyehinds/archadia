#include "ui/device/sdl2/Application.hpp"

namespace ui::device::sdl2 {

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

Result<Success> Application::run(int argc, const char* argv[])
{
	int sdlInitResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	if(0 != sdlInitResult) {
		return Result<Success>::makeFailureWithString(SDL_GetError());
	}

	int imgInitFlags = IMG_INIT_PNG;
	int imgInitResult = IMG_Init(imgInitFlags);

	if(imgInitFlags != imgInitResult) {
		return Result<Success>::makeFailureWithString(IMG_GetError());
	}

	int ttfInitResult = TTF_Init();
	if(0 != ttfInitResult) {
		return Result<Success>::makeFailureWithString(TTF_GetError());
	}

	muptSdlWindow.reset(
		SDL_CreateWindow(
			"My Window",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			320,
			240,
			SDL_WINDOW_SHOWN));

	if(!muptSdlWindow) {
		return Result<Success>::makeFailureWithString(SDL_GetError());
	}

	muptSdlRenderer.reset(
		SDL_CreateRenderer(
			muptSdlWindow.get(),
			-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

	if(!muptSdlRenderer)
	{
		return Result<Success>::makeFailureWithString(SDL_GetError());
	}
	
	m_uptUnRotatedTexture.reset(
		SDL_CreateTexture(
			muptSdlRenderer.get(),
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			240,
			320));
	
	ASSERT(m_uptUnRotatedTexture);

	m_uptRotatedTexture.reset(
		SDL_CreateTexture(
			muptSdlRenderer.get(),
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			320,
			240));	

	ASSERT(m_uptRotatedTexture);

	Result<Success> result = initialize(argc, argv);
	if (!result) {
		return result;
	}

	return renderLoop();
}

Result<Success> Application::renderLoop()
{
	SDL_Event sdlEvent = {};

	while(!mQuit)
	{
		if( 0 != SDL_PollEvent(&sdlEvent))
		{
			dispatchEvent(sdlEvent);
		}


		int r = SDL_SetRenderTarget(muptSdlRenderer.get(), m_uptUnRotatedTexture.get());
		ASSERT(0 == r);

		SDL_RenderClear(muptSdlRenderer.get());
		ASSERT(0 == r);
		
		RenderContext renderContext(*muptSdlRenderer);
		render(renderContext);

		r = SDL_SetRenderTarget(muptSdlRenderer.get(), m_uptRotatedTexture.get());
		ASSERT(0 == r);

		SDL_RenderClear(muptSdlRenderer.get());
		ASSERT(0 == r);

		SDL_Rect target {0, 0, 240, 320};
		SDL_Point center{120,120};		

		r = SDL_RenderCopyEx(muptSdlRenderer.get(), m_uptUnRotatedTexture.get(), nullptr, &target, 270.0, &center, SDL_FLIP_NONE);
		ASSERT(0 == r);

		r = SDL_SetRenderTarget(muptSdlRenderer.get(), nullptr);
		ASSERT(0 == r);

		r = SDL_RenderCopy(muptSdlRenderer.get(), m_uptRotatedTexture.get(), nullptr, nullptr);
		ASSERT(0 == r);

		SDL_RenderPresent(muptSdlRenderer.get());
		SDL_Delay(60);
	}

	return Result{Success{}};
}

Result<ref<Surface>> Application::loadSurface(
	CStr filePath)
{
	std::unique_ptr<SDL_Texture> uptSdlTexture(
		IMG_LoadTexture(
			muptSdlRenderer.get(),
			filePath.c_str()));

	if(!uptSdlTexture) {
		LogFmt( "Failed to load texture: %s\n", filePath.c_str());		
		return Result<ref<Surface>>::makeFailureWithString(IMG_GetError());
	}

	ref<Surface> refSurface = make_ref<Surface>(std::move(uptSdlTexture));

	return Result{std::move(refSurface)};
}

Result<ref<Font>> Application::loadFont(
	UnitType sizePx,
	CStr fileName)
{
	std::unique_ptr<TTF_Font> uptSdlFont(
		TTF_OpenFont(fileName.c_str(), sizePx));

	if(!uptSdlFont) {
		return Result<ref<Font>>::makeFailureWithString(TTF_GetError());
	}

	ref<Font> refFont = make_ref<Font>(std::move(uptSdlFont));

	return Result{std::move(refFont)};
}

Result<Success> Application::renderText(
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
		return Result<Success>::makeFailureWithString(TTF_GetError());
	}

	std::unique_ptr<SDL_Texture> uptSdlTexture(
		SDL_CreateTextureFromSurface(
			muptSdlRenderer.get(),
			uptSdlSurface.get()));

	if(!uptSdlTexture) {
		return Result<Success>::makeFailureWithString(SDL_GetError());
	}

	refSurface = make_ref<Surface>(std::move(uptSdlTexture));

	return Result{Success{}};
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

}