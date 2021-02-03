#pragma once

#include "Pi2Jamma/Configuration/Configuration.hpp"
#include "Pi2Jamma/screens/ScreenControllerStack.hpp"
#include "Pi2Jamma/screens/ScreenThemeDescription.hpp"
#include "ui/Application.hpp"
#include "ui/KeyDownEvent.hpp"
#include "ui/RenderContext.hpp"
#include "ui/elements/Console.hpp"
#include "ui/elements/Element.hpp"

class Pi2JammaApplication : public ui::Application
{

public:

	Pi2JammaApplication();

private:


	virtual Result initialize(int argc, const char* argv[]) override;

	virtual void render(ui::RenderContext& renderContext) override;

	virtual void keyDownEvent(const ui::KeyDownEvent& keyDownEvent) override;

	Result loadConfiguration();
	Result setupUi();

	Configuration mConfiguration;

	CStr mDataDir;
	std::string m_fullThemeDir;
	std::string mSnapsDir;

	ref<ui::Element> m_refRootElement;
	ref<ui::Console> mrefConsole;

	ScreenThemeDescription m_screenThemeDescription;
	ScreenControllerStack m_screenControllerStack;
};
