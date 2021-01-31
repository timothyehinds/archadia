#pragma once

template<typename T>
struct Arg
{
	using Type = const T&;
	using Reference = T&;
};

template<>
struct Arg<unsigned char>
{
	using Type = unsigned char;
	using Reference = unsigned char&;
};

template<>
struct Arg<unsigned long>
{
	using Type = unsigned long;
	using Reference = unsigned long&;
};

template<typename T >
struct Arg<T*>
{
	using Type = const T*;
	using Reference = T*;
};