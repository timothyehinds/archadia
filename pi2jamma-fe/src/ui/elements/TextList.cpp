
#include "ui/elements/TextList.hpp"

#include "ui/Application.hpp"
#include "ui/Crop.hpp"
#include "ui/Point.hpp"

namespace ui {

TextList::TextList(
	TextListModel& listModel,
	Element* pParent,
	const Rect& rect,
	ref<Font> refFont,
	const Color& unselectedColor,
	const Color& selectedColor,
	UnitType lineHeight,
	HorizontalAlignment horizontalAlignment,
	VerticalAlignment verticalAlignment)
	: Element(pParent, rect)
	, mListModel(listModel)
	, mrefFont(std::move(refFont))
	, mUnselectedColor(unselectedColor)
	, mSelectedColor(selectedColor)
	, mLineHeight(lineHeight)
	, mHorizontalAlignment(horizontalAlignment)	
	, mVerticalAlignment(verticalAlignment)
{
	auto numItems = mListModel.getNumItems();

	mLabels.reserve(numItems);

	for(size_t i = 0; i < numItems; i++)
	{
		ref<Surface> refSurface;

		mLabels.emplace_back(
			createSurface(
				mUnselectedColor,
				mListModel.getItem(i)));
	}

	if(numItems > 0 ) {
		setSelection(0);
	}

	resize(Size(0,0), rect.getSize());
}

void TextList::setSelection(int itemIndex)
{
	if(itemIndex == mSelectedItem) {
		return;
	}

	mSelectedItem = itemIndex;

	mrefSelectedSurface =
		createSurface(
			mSelectedColor,
			mListModel.getItem(mSelectedItem));

	mListModel.onHighlighted(mSelectedItem);
}

void TextList::input(InputEvent& inputEvent)
{
	if(inputEvent.getAction() == Action::Down) {
		down();
		return;
	}

	if(inputEvent.getAction() == Action::Up) {
		up();
		return;
	}

	if(inputEvent.getAction() == Action::Select) {
		select();
	}

}

void TextList::up()
{
	if (mSelectedItem <= 0 ) {
		return;
	}

	setSelection(mSelectedItem - 1);
}

void TextList::down()
{
	if( mSelectedItem < static_cast<int>(mListModel.getNumItems()) - 1 )
	{
		setSelection(mSelectedItem + 1);
	}

}

void TextList::select()
{
	mListModel.onSelect(mSelectedItem);
}

ref<Surface> TextList::createSurface(
	const Color& color,
	CStr text)
{
	ref<Surface> refSurface;

	Result r =
		Application::get().renderText(
			refSurface,
			mrefFont,
			color,
			text.c_str());

	r.ignore();

	return refSurface;
}


void TextList::render(RenderContext& renderContext)
{
	UnitType centerY = getRect().getYCenter();

	UnitType x = getX();
	UnitType y = centerY - (mLineHeight / 2);
	y -= ((mNumItemsToDisplay / 2) * mLineHeight);

	int numItems = mLabels.size();

	int firstItemToDisplay = mSelectedItem - (mNumItemsToDisplay / 2);
	int lastItemToDisplay = firstItemToDisplay + mNumItemsToDisplay;

	for(int i = firstItemToDisplay; i < lastItemToDisplay; i ++ ) 
	{
		Point position(x,y);
		y += mLineHeight;

		if (i < 0) {
			continue;
		}

		if(i >= numItems) {
			break;
		}

		auto refSurface = mLabels[i];

		if(i == mSelectedItem) {
			refSurface = mrefSelectedSurface;
		}

		Rect targetRect(
			position,
			Size(getWidth(), mLineHeight));

		auto fitResult =
			fitRect(
				Rect(
					Point(0, 0),
					refSurface->getSize()),
				targetRect,
				CropMode::None,
				HorizontalAlignment::Left,
				VerticalAlignment::Center,
				mHorizontalAlignment,
				mVerticalAlignment);

		renderContext.draw(
			refSurface,
			fitResult.getTargetRect(),
			fitResult.getSourceRect());
	}
}

void TextList::resize(const Size& oldSize, const Size& newSize)
{
	mNumItemsToDisplay = newSize.getHeight() / mLineHeight;

	if(0 ==(mNumItemsToDisplay %2)) {
		mNumItemsToDisplay--;
	}

	mNumItemsToDisplay = std::max(mNumItemsToDisplay, 1);
}

}

