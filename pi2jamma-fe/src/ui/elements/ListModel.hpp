#pragma once

namespace ui {

template<typename ElementType>
class ListModel
{
public:
	virtual ~ListModel() = default;

	virtual size_t getNumItems() const = 0;
	virtual ElementType getItem(size_t index) const = 0;
	virtual void onHighlighted(size_t newSelection) = 0;
	virtual void onSelect(size_t newSelection) = 0;

};

}