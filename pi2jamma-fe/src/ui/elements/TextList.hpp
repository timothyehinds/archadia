#pragma once

#include "ui/BitmapFont.hpp"
#include "ui/Color.hpp"
#include "ui/Font.hpp"
#include "ui/Surface.hpp"
#include "ui/HorizontalAlignment.hpp"
#include "ui/VerticalAlignment.hpp"
#include "ui/elements/Element.hpp"
#include "ui/elements/ListModel.hpp"

#include <vector>

namespace ui {

using TextListModel = ListModel<CStr>;

class TextList final : public Element
{
public:

	TextList(
		TextListModel& listModel,
		Element* pParent,
		const Rect& rect,
		ref<BitmapFont> refSelectedBitmapFont,
		ref<BitmapFont> refUnselectedBitmapFont,
		const Color& unselectedColor,
		const Color& selectedColor,
		UnitType lineHeight,		
		HorizontalAlignment = HorizontalAlignment::Center,
		VerticalAlignment = VerticalAlignment::Center);

	void setSelection(int itemIndex);

	void up();
	void down();
	void select();

protected:

	virtual void render(RenderContext& renderContext) override;
	virtual void resize(const Size& oldSize, const Size& newSize) override;
	virtual void input(InputEvent& inputEvent) override;

private:

	TextListModel& mListModel;
	ref<BitmapFont> mrefUnselectedFont;
	ref<BitmapFont> mrefSelectedFont;
	Color mUnselectedColor;
	Color mSelectedColor;
	UnitType mLineHeight;
	HorizontalAlignment mHorizontalAlignment;
	VerticalAlignment mVerticalAlignment;

	ref<Surface> mrefSelectedSurface;

	int mSelectedItem = -1;
	int mNumItemsToDisplay = 0;
};

}

