#include "Pi2Jamma/screens/GameDatabase.hpp"

namespace fs = std::filesystem;

Game::Game(std::string name)
    : m_name{std::move(name)}
{
}

GameCore::GameCore(fs::path path)
    : m_path{std::move(path)}
{
     Result<Success> r{
        m_infoFile.load(
            fs::path(m_path).replace_extension(".info"))};

    m_romsFolder = m_path.filename().stem().string();
    size_t res{m_romsFolder.find("_libretro")};
    if (res != std::string::npos)
    {
        m_romsFolder.erase(res, m_romsFolder.length());
    }
}

const char* GameCore::getDisplayName() const
{
    auto it{m_infoFile.m_keyValueMap.find("display_name")};

    if (it == m_infoFile.m_keyValueMap.end())
    {
        return "?";
    }

    return it->second.c_str();
}

const char* GameCore::getRomsFolder() const
{
    return m_romsFolder.c_str();
}

void Games::load(const char* pCoreFolderPath, const char* pRomsFolderPath)
{
    std::error_code errorCode;

    for (const fs::directory_entry& dirEntry : fs::directory_iterator(pCoreFolderPath, errorCode))
    {
        if (dirEntry.path().extension() == ".so")
        {
            std::unique_ptr<GameCore> uptGameCore{std::make_unique<GameCore>(dirEntry.path())};

            fs::path romsPath{fs::path{"./../roms"} / uptGameCore->getRomsFolder()};

            std::error_code errorCode;

            for (const fs::directory_entry& dirEntry : fs::recursive_directory_iterator(romsPath, errorCode))
            {
                uptGameCore->m_games.push_back(
                    std::make_unique<Game>(dirEntry.path().filename()));
            }

            m_cores.push_back(std::move(uptGameCore));            
        }
    }
}

