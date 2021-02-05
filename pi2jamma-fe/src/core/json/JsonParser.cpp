#if 0
#include "core/json/JsonParser.hpp"
#include "core/json/Parser.hpp"
#include "file/File.hpp"

Result JsonLoadFromFile( Json& json, const char* pFilename )
{
    File file;
    Result r = file.open(pFilename, File::OpenMode::Read);
    if(!r) {
        return r;
    }

    FileSize fileSize = 0;
    r = file.getSize(fileSize);
    if(!r) {
        return r;
    }

    std::vector<char> s;
    s.resize(fileSize);

    r = file.readExactly(s.data(), s.size());
    if (!r) {
        return r;
    }

    LogFmt("starting parse..%d\n", (int)fileSize);

    StringParserAssembly stringParserAssembly(s.data(), s.size());
    auto pParser = stringParserAssembly.GetParser();

    JsonParser<StringParserAssembly::ParserType> jsonParser(pParser);

    bool result = jsonParser.Parse(&json);
    if (!result) {
        return
            Result::makeFailureWithString(
                formatString(
                    "JSON ERROR: %s Line: %d Column: %d\n",
                    jsonParser.GetError().c_str(),
                    (int) pParser->GetLineCount(),
                    (int) pParser->GetColumnCount()));              

    }
    
    return Result::makeSuccess();
};

#endif
