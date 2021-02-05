#pragma once

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

    std::filesystem::path m_path;
    std::vector<std::unique_ptr<Game>> m_games;
    std::map<std::string, std::string> m_infoMap;
};

class Games
{
public:
    void load(const char* pCoreFolderPath, const char* pRomsFolderPath);

    std::vector<std::unique_ptr<GameCore>> m_cores;
};