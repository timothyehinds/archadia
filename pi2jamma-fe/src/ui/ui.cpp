#include "ui/ui.hpp"

#include "core/Char.hpp"
#include "core/debug.hpp"
#include "core/Util.hpp"
#include "core/StringSpan.hpp"
#include "core/meta/Meta.hpp"

#include "ui/Color.hpp"
#include "ui/Key.hpp"
#include "ui/Rect.hpp"
#include "ui/HorizontalAlignment.hpp"
#include "ui/Orientation.hpp"
#include "ui/VerticalAlignment.hpp"

namespace ui
{

void initialize() {
	META_ENUM(Key);
	META_ENUM_VALUE(Key, DownArrow);
	META_ENUM_VALUE(Key, UpArrow);
	META_ENUM_VALUE(Key, LeftArrow);
	META_ENUM_VALUE(Key, RightArrow);	
	META_ENUM_VALUE(Key, Escape);
	META_ENUM_VALUE(Key, Space);

	META_CLASS(Rect);
	META_CLASS_PROPERTY(Rect, X);
	META_CLASS_PROPERTY(Rect, Y);
	META_CLASS_PROPERTY(Rect, Width);
	META_CLASS_PROPERTY(Rect, Height);

	META_CLASS(Color);
	META_CLASS_PROPERTY(Color, Red);
	META_CLASS_PROPERTY(Color, Green);
	META_CLASS_PROPERTY(Color, Blue);
	META_CLASS_PROPERTY(Color, Alpha);

	META_ENUM(HorizontalAlignment);
	META_ENUM_VALUE(HorizontalAlignment, Left);
	META_ENUM_VALUE(HorizontalAlignment, Center);
	META_ENUM_VALUE(HorizontalAlignment, Right);

	META_ENUM(VerticalAlignment);
	META_ENUM_VALUE(VerticalAlignment, Top);
	META_ENUM_VALUE(VerticalAlignment, Center);
	META_ENUM_VALUE(VerticalAlignment, Bottom);	

	META_ENUM(Orientation);
	META_ENUM_VALUE(Orientation, Landscape);
	META_ENUM_VALUE(Orientation, Portrait);
}

template<typename T >
static Result parseHex(T& t, StringSpan s) {
	t = static_cast<T>(0);
	for( auto&& c : s ) {
		T hexValue = 0;
		bool ok = OmCharToHexValue(c, &hexValue);
		if(!ok) {
			return Result::makeFailureWithString(formatString("%c is not a valid hex value.", c));
		}

		t = t << 4;
		t += hexValue;
 	}

 	return Result::makeSuccess();
}

Result parseColor(Color& color, StringSpan stringSpan)
{
	if(stringSpan.size() < 0) {
		return Result::makeFailureWithStringLiteral("Empty String.");
	}

	if('#' != stringSpan[0]) {
		return Result::makeFailureWithStringLiteral("Expected color string to start with '#");
	}

	if(! oneOf<size_t>(stringSpan.size(), {7, 9})) {
		return Result::makeFailureWithStringLiteral("Weird number of characters in hext color string.");
	}

	auto iter = stringSpan.begin();
	iter++;
	auto end = stringSpan.end();

	std::array<uint8_t, 4 > mComponents;
	mComponents[3] = 0xFF;
	size_t i = 0;

	while(iter < end) {
		Result r = parseHex(mComponents[i], StringSpan(iter, iter + 2));
		if(r.peekFailed()) {
			return r;
		}
		iter += 2;
		i++;
	}

	color = Color(mComponents);

	return Result::makeSuccess();
}

} // namespace ui

Result Serializer<ui::Color>::load(ui::Color& color, ObjectReadStream& readStream)
{
	bool isObject = false;
	Result r = readStream.peekObject(isObject);
	if(r.peekFailed()) {
		return r;
	}

	if(isObject) {
		return Meta::get().findType<ui::Color>()->load(&color, readStream);
	}

	bool isString = false;
	r = readStream.peekString(isString);
	if(r.peekFailed()) {
		return r;
	}

	if(isString) {
		std::string str;
		r = readStream.readString(str);
		if(r.peekFailed()) {
			return r;
		}
		return ui::parseColor(color, str);
	}

	return Result::makeFailureWithStringLiteral("Bad color object.");
}

Result Serializer<ui::Color>::save(const ui::Color& color, ObjectWriteStream& writeStream)
{
	return Meta::get().findType<ui::Color>()->save(&color, writeStream);
}
