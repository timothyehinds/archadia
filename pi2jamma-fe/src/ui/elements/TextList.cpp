
#include "ui/elements/TextList.hpp"

#include "ui/Application.hpp"
#include "ui/Crop.hpp"
#include "ui/Point.hpp"

namespace ui {

TextList::TextList(
	TextListModel& listModel,
	Element* pParent,
	const Rect& rect,
	ref<BitmapFont> refSelectedBitmapFont,
	ref<BitmapFont> refUnselectedBitmapFont,
	const Color& unselectedColor,
	const Color& selectedColor,
	UnitType lineHeight,
	HorizontalAlignment horizontalAlignment,
	VerticalAlignment verticalAlignment)
	: Element(pParent, rect)
	, mListModel(listModel)
	, mrefUnselectedFont(std::move(refUnselectedBitmapFont))
	, mrefSelectedFont(std::move(refSelectedBitmapFont))
	, mUnselectedColor(unselectedColor)
	, mSelectedColor(selectedColor)
	, mLineHeight(lineHeight)
	, mHorizontalAlignment(horizontalAlignment)	
	, mVerticalAlignment(verticalAlignment)
{
	auto numItems = mListModel.getNumItems();

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

void TextList::render(RenderContext& renderContext)
{
	UnitType centerY = getRect().getYCenter();

	UnitType x = getX();
	UnitType y = centerY - (mLineHeight / 2);
	y -= ((mNumItemsToDisplay / 2) * mLineHeight);

	int numItems = mListModel.getNumItems();

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

		Rect targetRect(
			position,
			Size(getWidth(), mLineHeight));
		
		ref<ui::BitmapFont> refFont =
			(i == mSelectedItem)
				? mrefSelectedFont
				: mrefUnselectedFont;

		refFont->render(
			renderContext,
			mListModel.getItem(i),
			targetRect);


		#if 0
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
		#endif
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

