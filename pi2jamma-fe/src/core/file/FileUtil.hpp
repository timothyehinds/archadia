#pragma once

#include "core/CString.hpp"
#include "core/Result.hpp"

#include <vector>

Result<Success> readFile(CStr filename, std::vector<char>& data);