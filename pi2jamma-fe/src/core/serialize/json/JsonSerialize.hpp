#pragma once

#include "core/file/File.hpp"
#include "core/file/FileUtil.hpp"
#include "core/parse/Parser.hpp"
#include "core/parse/StringDecoder.hpp"
#include "core/serialize/json/JsonReadStream.hpp"
#include "core/serialize/json/JsonWriteStream.hpp"

template<typename T>
Result loadJson(T& t, CStr fileName)
{
	std::vector<char> fileData;
	Result r = readFile(fileName, fileData);
	if(r.peekFailed()){
		return r;
	}

	const char* pData = fileData.data();

	StringDecoder dataStream(pData, pData + fileData.size());
	std::vector<char> workArea;
	
	OmParser<StringDecoder> parser(
		dataStream,
		workArea,
		fileName);

	JsonReadStream<OmParser<StringDecoder>> jsonReadStream(parser);

	return load(t, jsonReadStream);
};

template<typename T>
Result saveJson(const T& t, CStr fileName)
{
	File file;
	Result r = file.open(fileName, File::OpenMode::Write);
	if(r.peekFailed()) {
		return r;
	}

	JsonWriteStream<File> jsonWriteStream;

	return save(t, jsonWriteStream);
}

class StringWriteStream
{
public:

	StringWriteStream( std::string& s)
		: mString(s)
	{}

	Result write(const void* pVoidData, size_t numBytes) {
		const char* pCharData =
			reinterpret_cast<const char*>(pVoidData);

		mString.insert(
			mString.size(),
			pCharData,
			numBytes);

		return Result::makeSuccess();
	}

private:
	std::string& mString;
};

template<typename T>
std::string dump(const T& t)
{
	std::string s;
	StringWriteStream stringWriteStream(s);
	JsonWriteStream<StringWriteStream>
		jsonWriteStream(stringWriteStream);
	Result r = save(t, jsonWriteStream);
	r.catastrophic();
	return s;
}

