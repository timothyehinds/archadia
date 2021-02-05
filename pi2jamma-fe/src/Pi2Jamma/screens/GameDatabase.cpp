#include "Pi2Jamma/screens/GameDatabase.hpp"

namespace fs = std::filesystem;

Game::Game(std::string name)
    : m_name{std::move(name)}
{
}

GameCore::GameCore(fs::path path)
    : m_path{std::move(path)}
{
    
}

void Games::load(const char* pCoreFolderPath, const char* pRomsFolderPath)
{
    std::error_code errorCode;

    for (const fs::directory_entry& dirEntry : fs::directory_iterator(pCoreFolderPath, errorCode))
    {
        if (dirEntry.path().extension() == ".so")
        {
            m_cores.push_back(std::make_unique<GameCore>(dirEntry.path()));
        }
    }

    for (const std::unique_ptr<GameCore>& uptGameCore : m_cores)
    {
        for (const fs::directory_entry& dirEntry : fs::directory_iterator(pCoreFolderPath, errorCode))
        {
            uptGameCore->m_games.push_back(
                std::make_unique<Game>(dirEntry.path().filename()));
       }
    }
}

