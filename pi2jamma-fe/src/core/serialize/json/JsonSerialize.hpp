#pragma once

#include "core/file/File.hpp"
#include "core/file/FileUtil.hpp"
#include "core/parse/Parser.hpp"
#include "core/parse/StringDecoder.hpp"
#include "core/serialize/json/JsonReadStream.hpp"
#include "core/serialize/json/JsonWriteStream.hpp"

template<typename T>
Result<Success> loadJson(T& t, CStr fileName)
{
	std::vector<char> fileData;
	Result<Success> r = readFile(fileName, fileData);
	if(!r){
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
Result<Success> saveJson(const T& t, CStr fileName)
{
	File file;
	Result<Success> r = file.open(fileName, File::OpenMode::Write);
	if(!r) {
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

	Result<Success> write(const void* pVoidData, size_t numBytes) {
		const char* pCharData =
			reinterpret_cast<const char*>(pVoidData);

		mString.insert(
			mString.size(),
			pCharData,
			numBytes);

		return Result{Success{}};
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
	Result<Success> r = save(t, jsonWriteStream);
	r.catastrophic();
	return s;
}

