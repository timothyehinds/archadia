#pragma once

#include "core/Result.hpp"

class System
{
	public:

	 	[[nodiscard]] static Result<Success> exec(const char* cmd, std::string& output);
};