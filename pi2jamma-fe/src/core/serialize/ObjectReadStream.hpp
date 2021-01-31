#pragma once

#include "core/Result.hpp"

class ObjectReadStream
{
public:

	virtual ~ObjectReadStream() = default;

	virtual Result readNativeInteger(int64_t& integer) = 0;
	virtual Result readNativeFloat(double& flt) = 0;
	virtual Result readString(std::string& str) = 0;
	virtual Result readBoolean(bool& boolean) = 0;
	virtual Result readCVariableName(std::string& str) = 0;

	virtual Result beginArray() = 0;
	virtual Result nextArrayItem(bool& gotItem) = 0;
	virtual Result endArray() = 0;

	virtual Result peekObject(bool& isObject) = 0;
	virtual Result peekString(bool& isString) = 0;
	
	virtual Result beginObject() = 0;
	virtual Result beginField(bool& done, std::string& name) = 0;
	virtual Result endField() = 0;
	virtual Result endObject() = 0;

	virtual Result makeError(CStr text) = 0;

	template<typename T>
	Result readInteger(T& t) {
		int64_t i= 0;
		Result r = readNativeInteger(i);
		if(r.peekFailed()) {
			return r;
		}

		t = static_cast<T>(i);
		return Result::makeSuccess();
	}

	template<typename T>
	Result readFloat(T& t) {
		double d = 0.0;
		Result r = readNativeFloat(d);
		if(r.peekFailed()) {
			return r;
		}

		t = static_cast<T>(d);

		return Result::makeSuccess();
	}
};