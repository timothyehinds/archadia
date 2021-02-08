#pragma once

#include "Pi2Jamma/retroarch/RetroarchConfigFile.hpp"

#include <string>
#include <vector>
#include <memory>

#include <filesystem>
#include <map>

class Game
{
public:
    Game(std::string name);
    
    std::string m_name;
};

class GameCore
{
public:
    GameCore(std::filesystem::path);
    const char* getDisplayName() const;
    const char* getRomsFolder() const;
//private:

    std::filesystem::path m_path;
    std::string m_romsFolder;
    std::vector<std::unique_ptr<Game>> m_games;

    RetroarchConfigFile m_infoFile;
};

class Games
{
public:
    void load(const char* pCoreFolderPath, const char* pRomsFolderPath);

    std::vector<std::unique_ptr<GameCore>> m_cores;
};