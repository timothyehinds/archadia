#pragma once

#include "core/Arg.hpp"
#include "core/debug.hpp"
#include <typeinfo>

template<typename T>
struct Comparer
{
	static int Compare(
		typename Arg<T>::Type t1,
		typename Arg<T>::Type t2)
	{
		if(t1 < t2) {
			return -1;
		}
		else if(t1 > t2) {
			return 1;
		}

		return 0;
	}
};

template<>
struct Comparer<std::type_info>
{
	static int Compare(
		const std::type_info& t1,
		const std::type_info& t2)
	{
		if (t1 == t2 ) {
			return 0;
		}
		else if(t1.before(t2))
		{
			return-1;
		}

		return 1;
	}
};