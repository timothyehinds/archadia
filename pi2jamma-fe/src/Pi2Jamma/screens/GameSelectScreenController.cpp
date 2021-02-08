#include "Pi2Jamma/screens/GameSelectScreenController.hpp"

#include <filesystem>

GameSelectScreenController::GameSelectScreenModel::GameSelectScreenModel(GameSelectScreenController& controller)
	: m_controller(controller)
{
}

CStr GameSelectScreenController::GameSelectScreenModel::getTitle() 
{
	return "Games";
}

size_t GameSelectScreenController::GameSelectScreenModel::getNumItems()
{
	return m_controller.m_gameDatabase.m_cores.size();
}

CStr GameSelectScreenController::GameSelectScreenModel::getItemText(const size_t i)
{
	return m_controller.m_gameDatabase.m_cores[i]->getRomsFolder();
}

ref<ui::Surface> GameSelectScreenController::GameSelectScreenModel::getItemSurface(const size_t i)
{
	return nullptr;
}

void GameSelectScreenController::GameSelectScreenModel::onItemSelected(const size_t i)
{
	

}

//----------------------------------------------------------------------------------------------------------------------

GameSelectScreenController::GameSelectScreenController(
	ref<ScreenTheme> refScreenTheme)
	: m_screenModel{*this}
	, m_refScreenTheme{std::move(refScreenTheme)}
{
	m_gameDatabase.load("/home/x/.config/retroarch/cores", "./roms");
}

ref<ui::Element> GameSelectScreenController::onActivate(
	ui::Element* pParent,
	const ui::Rect& rect)
{
	m_refScreen = make_ref<Screen>(pParent, rect, m_refScreenTheme, m_screenModel);
	return m_refScreen;
}

void GameSelectScreenController::onDeactivate()
{
	m_refScreen = nullptr;
}