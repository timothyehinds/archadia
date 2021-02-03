#include "Pi2Jamma/screens/GameSelectScreenController.hpp"

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
	return 1;
}

CStr GameSelectScreenController::GameSelectScreenModel::getItemText(const size_t i)
{
	return "Pac Man";
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
}

ref<ui::Element> GameSelectScreenController::activate(
	ui::Element* pParent,
	const ui::Rect& rect)
{
	m_refScreen = make_ref<Screen>(pParent, rect, m_refScreenTheme, m_screenModel);
	return m_refScreen;
}

void GameSelectScreenController::deactivate()
{
	m_refScreen = nullptr;
}