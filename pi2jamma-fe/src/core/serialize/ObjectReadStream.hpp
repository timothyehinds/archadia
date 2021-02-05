#pragma once

#include "core/Result.hpp"

class ObjectReadStream
{
public:

	virtual ~ObjectReadStream() = default;

	virtual Result<Success> readNativeInteger(int64_t& integer) = 0;
	virtual Result<Success> readNativeFloat(double& flt) = 0;
	virtual Result<Success> readString(std::string& str) = 0;
	virtual Result<Success> readBoolean(bool& boolean) = 0;
	virtual Result<Success> readCVariableName(std::string& str) = 0;

	virtual Result<Success> beginArray() = 0;
	virtual Result<Success> nextArrayItem(bool& gotItem) = 0;
	virtual Result<Success> endArray() = 0;

	virtual Result<Success> peekObject(bool& isObject) = 0;
	virtual Result<Success> peekString(bool& isString) = 0;
	
	virtual Result<Success> beginObject() = 0;
	virtual Result<Success> beginField(bool& done, std::string& name) = 0;
	virtual Result<Success> endField() = 0;
	virtual Result<Success> endObject() = 0;

	virtual Result<Success> makeError(CStr text) = 0;

	template<typename T>
	Result<Success> readInteger(T& t) {
		int64_t i= 0;
		Result<Success> r = readNativeInteger(i);
		if(!r)
		{
			return r;
		}

		t = static_cast<T>(i);
		return Result{Success{}};
	}

	template<typename T>
	Result<Success> readFloat(T& t) {
		double d = 0.0;
		Result<Success> r = readNativeFloat(d);
		if(!r)
		{
			return r;
		}

		t = static_cast<T>(d);

		return Result{Success{}};
	}
};