    #include "Pi2Jamma/retroarch/RetroarchConfigFile.hpp"

    #include "core/file/FileUtil.hpp"
    #include "core/parse/StringDecoder.hpp"
    #include "core/parse/Parser.hpp"
    #include "core/parse/Parse.hpp"

    Result<Success> RetroarchConfigFile::load(const std::filesystem::path& path)
    {
        std::vector<char> data;

        Result<Success> r{readFile(path.c_str(), data)};

        if (!r)
        {
            return r;
        }

        if (data.empty())
        {
            return Result<Success>::makeFailureWithString(path.string() + "is Empty.");
        }

        StringDecoder stringDecoder{data.data(), data.data() + data.size()};

	    std::vector<char> workArea;
        OmParser<StringDecoder> parser{stringDecoder, workArea, path.c_str()};
    
        std::string key;
        std::string value;

        while(!parser.Eof())
        {
            key.clear();

            OmParseEatWhite(&parser);

            if (!OmParseTokenWithCharDelimiters(&parser, &key, "\t ="))
            {
                return Result<Success>::makeFailureWithString(parser.GetErrorHint());                
            }

            OmParseEatWhite(&parser);

            if (!OmParseMatch(&parser, '='))
            {
                return Result<Success>::makeFailureWithString(parser.GetErrorHint());
            }

            OmParseEatWhite(&parser);

            OmParseMatchQuotedString(&parser, &value);

            OmParseEatWhite(&parser);

            std::string keyString{key.begin(), key.end()};
 
            m_keyValueMap.insert(std::make_pair(std::move(keyString), std::move(value)));
        }

        return Result{Success{}};
    }