#pragma once

#include "core/Result.hpp"

class System
{
	public:

	 	[[nodiscard]] static Result exec(const char* cmd, std::string& output);
};