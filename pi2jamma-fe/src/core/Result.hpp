#pragma once

#include "debug.hpp"
#include <functional>
#include <string>
#include <variant>

struct Success{};

template<typename T>
class Result final
{
public:

	using InfoFunction = std::function<const char*(std::string&)>;

	Result(T&&);
	Result(const T&);
	Result(Result&&) = default;
	Result(const Result&) = default;

	Result(InfoFunction&& infoFunction);	

	Result& operator=(const Result& result) = default;
	Result& operator=(Result&& rhs) = default;	
	
	static Result makeSuccess(T&& t);
	static Result makeFailureWithStringLiteral(const char* msg);
	static Result makeFailureWithString(std::string msg);
	static Result makeFailureNotImplemented();

	~Result();

	void catastrophic();

	const char* getMessage();
	const T& operator->();
	const T& operator*();

	T&& moveResult();

	InfoFunction&& moveError();

	explicit operator bool();

private:

	std::variant<T, InfoFunction> m_variant;
};

template<typename T>
inline Result<T> Result<T>::makeSuccess(T && t)
{
	return
		Result(std::forward<T&&>(t));
}

template<typename T>
inline Result<T> Result<T>::makeFailureWithStringLiteral(const char* msg)
{
	return 
		Result(
			[msg](std::string& workArea) {
				return msg;
			} );
}


template<typename T>
inline Result<T> Result<T>::makeFailureWithString(std::string msg)
{
	return
		Result(
			[m = std::move(msg)](std::string& workArea) {
				return m.c_str();
			} );

}

template<typename T>
inline Result<T> Result<T>::makeFailureNotImplemented()
{
	return makeFailureWithStringLiteral("Not Implemented.");
}

template<typename T>
inline Result<T>::~Result()
{
}

template<typename T>
inline void Result<T>::catastrophic()
{
	ASSERTMSG(*this, getMessage());
}

template<typename T>
inline const char* Result<T>::getMessage()
{
	if (std::holds_alternative<InfoFunction>(m_variant))
	{
		std::string workArea;
		return std::get<InfoFunction>(m_variant)(workArea);
	}

	return "Success";
}

template<typename T>
const T& Result<T>::operator->()
{
	return std::get<T>(m_variant);
}

template<typename T>
const T& Result<T>::operator*()
{
	return std::get<T>(m_variant);
}

template<typename T>
T&& Result<T>::moveResult()
{
	return std::get<T>(m_variant);
}

template<typename T>
typename Result<T>::InfoFunction&& Result<T>::moveError()
{
	return std::move(std::get<InfoFunction>(m_variant));
}

template<typename T>
inline Result<T>::Result(T&& t)
	: m_variant{std::move(t)}
{
}
template<typename T>
inline Result<T>::Result(const T& t)
	: m_variant{t}
{
}


template<typename T>
inline Result<T>::Result(InfoFunction&& infoFunction)
	: m_variant(std::move(infoFunction))
{
}

template<typename T>
inline Result<T>::operator bool()
{
	return std::holds_alternative<T>(m_variant);
}