#pragma once

#include "core/Result.hpp"

#include <filesystem>
#include <vector>

class RetroarchConfigFile
{
public:
    [[nodiscard]] Result<Success> load(const std::filesystem::path&);
    
private:

    std::vector<char> m_fileData;
};