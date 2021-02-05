#pragma once

#include "Pi2Jamma/Configuration/Configuration.hpp"
#include "Pi2Jamma/screens/ScreenControllerStack.hpp"
#include "Pi2Jamma/screens/ScreenThemeDescription.hpp"
#include "ui/Application.hpp"
#include "ui/KeyDownEvent.hpp"
#include "ui/RenderContext.hpp"
#include "ui/elements/Console.hpp"
#include "ui/elements/Element.hpp"

#include <filesystem>

class Pi2JammaApplication : public ui::Application
{
public:
	Pi2JammaApplication();

private:
	virtual Result<Success> initialize(int argc, const char* argv[]) override;

	virtual void render(ui::RenderContext& renderContext) override;

	virtual void keyDownEvent(const ui::KeyDownEvent& keyDownEvent) override;

	Result<Success> loadConfiguration();
	Result<Success> setupUi();

	Configuration mConfiguration;

	std::filesystem::path m_dataPath;
	std::filesystem::path m_fullThemePath;

	ref<ui::Element> m_refRootElement;
	ref<ui::Console> mrefConsole;

	ScreenThemeDescription m_screenThemeDescription;
	std::unique_ptr<ScreenControllerStack> m_uptScreenControllerStack;
};
