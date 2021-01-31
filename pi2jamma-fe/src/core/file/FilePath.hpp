#pragma once

#include "core/StringSpan.hpp"
#include <vector>
#include <string>

#define PATH_SEP_STR "/"
const char PATH_SEP_CHAR = '/';

std::string joinPath(const std::initializer_list<StringSpan>& parts);
std::vector<std::string> splitPath(StringSpan stringSpan);
std::string getPathEntryName(StringSpan stringSpan);

inline std::string joinPath(
	StringSpan s1,
	StringSpan s2)
{
	return joinPath({s1,s2});
}

inline std::string joinPath(
	StringSpan s1,
	StringSpan s2,
	StringSpan s3)
{
	return joinPath({s1,s2,s3});
}
inline std::string joinPath(
	StringSpan s1,
	StringSpan s2,
	StringSpan s3,
	StringSpan s4)
{
	return joinPath({s1,s2,s3,s4});
}

