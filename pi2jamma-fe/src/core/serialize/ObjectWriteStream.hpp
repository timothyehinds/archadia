#pragma once

#include "core/Result.hpp"
#include "core/StringSpan.hpp"

class ObjectWriteStream
{
public:
	virtual ~ObjectWriteStream() = default;

	virtual Result<Success> writeNativeFloat(double flt) = 0;
	virtual Result<Success> writeNativeInt(int64_t i) = 0;
	virtual Result<Success> writeBoolean(bool b) = 0;
	virtual Result<Success> writeCVariableName(StringSpan s) = 0;
	virtual Result<Success> writeString(StringSpan s) = 0;

	virtual Result<Success> beginObject() = 0;
	virtual Result<Success> beginField(StringSpan s) = 0;
	virtual Result<Success> endField() = 0;
	virtual Result<Success> endObject() = 0;

	virtual Result<Success> beginArray() = 0;
	virtual Result<Success> beginArrayItem() = 0;
	virtual Result<Success> endArrayItem() = 0;
	virtual Result<Success> endArray() = 0;
	
	template<typename T>
	Result<Success> writeFloat(T t) {
		return writeNativeFloat(static_cast<double>(t));
	}

	template<typename T>
	Result<Success> writeInteger(T t) {
		return writeNativeInt(static_cast<uint64_t>(t));
	}
};