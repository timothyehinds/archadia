#pragma once

#include "core/Result.hpp"

#include <map>
#include <filesystem>
#include <vector>

class RetroarchConfigFile
{
public:
    [[nodiscard]] Result<Success> load(const std::filesystem::path&);
    
///private:

    std::vector<char> m_fileData;
   
    std::map<std::string, std::string> m_keyValueMap;
};