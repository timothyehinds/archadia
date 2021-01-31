#pragma once

#include "debug.hpp"
#include <functional>
#include <string>

class Result final
{
public:

	static Result makeSuccess();
	static Result makeFailureWithStringLiteral(const char* msg);
	static Result makeFailureWithString(std::string msg);
	static Result makeFailureNotImplemented();

	Result(Result&& result);
	~Result();

	Result& operator=(const Result& result) = delete;
	Result& operator=(Result&& rhs);

	bool succeeded();
	bool failed();
	void catastrophic();

	bool peekFailed();
	bool peekSucceeded();

	void ignore();

	const char* getMessage();

private:

	using InfoFunction = std::function<const char*(std::string&)>;

	Result(bool success, InfoFunction&& infoFunction);

	bool mSucceeded : 1;
	bool mWasChecked : 1;

	InfoFunction mInfoFunction;
};

inline Result Result::makeSuccess()
{
	return
		Result(
			true,
			[](std::string& s) {
				return "OK";
			} );
}

inline Result Result::makeFailureWithStringLiteral(const char* msg)
{
	return 
		Result(
			false, 
			[msg](std::string& workArea) {
				return msg;
			} );
}

inline Result Result::makeFailureWithString(std::string msg)
{
	return
		Result(
			false,
			[m = std::move(msg)](std::string& workArea) {
				return m.c_str();
			} );

}

inline Result Result::makeFailureNotImplemented()
{
	return makeFailureWithStringLiteral("Not Implemented.");
}

inline Result::Result(Result&& rhs)
	: mSucceeded(rhs.mSucceeded)
	, mWasChecked(false)
	, mInfoFunction(std::move(rhs.mInfoFunction))
{
	rhs.mWasChecked = true;
}


inline Result& Result::operator=(Result&& rhs)
{
	ASSERT(mWasChecked);

	mSucceeded = rhs.mSucceeded;
	mWasChecked = false;
	mInfoFunction = std::move(rhs.mInfoFunction);
	rhs.mWasChecked = true;

	return (*this);
}

inline Result::~Result()
{
	ASSERTMSG(mWasChecked, getMessage());
}

inline bool Result::succeeded()
{
	mWasChecked = true;
	return mSucceeded;
}

inline bool Result::failed()
{
	mWasChecked = true;
	return !mSucceeded;
}

inline bool Result::peekFailed()
{
	if(!mSucceeded)
	{
		return true;
	}

	mWasChecked = true;

	return false;
}

inline bool Result::peekSucceeded()
{
	return mSucceeded;
}

inline void Result::ignore()
{
	mWasChecked = true;
}

inline void Result::catastrophic()
{
	ASSERTMSG(succeeded(), getMessage());
}

inline const char* Result::getMessage()
{
	std::string workArea;
	return mInfoFunction(workArea);
}

inline Result::Result(bool success, InfoFunction&& infoFunction)
	: mSucceeded(success)
	, mWasChecked(false)
	, mInfoFunction(std::move(infoFunction))
{

}