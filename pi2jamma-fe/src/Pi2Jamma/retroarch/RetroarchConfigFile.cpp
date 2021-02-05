    #include "Pi2Jamma/retroarch/RetroarchConfigFile.hpp"

    #include "core/file/File.hpp"

    Result<Success> RetroarchConfigFile::load(const std::filesystem::path& path)
    {
        File file;

        if (file.open(path.c_str(), File::OpenMode::Read))
        {
            if (Result<FileSize> resFileSize{file.getSize()})
            {
                m_fileData.resize(*resFileSize);

                if (!file.readExactly(&m_fileData[0], *resFileSize))
                {
                    m_fileData.clear();
                }
            }
        }

        return Result{Success{}};
    }