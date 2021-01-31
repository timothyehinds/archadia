#pragma once

#include "core/Result.hpp"

class System
{
	public:

	 	static Result exec(const char* cmd, std::string& output);
};