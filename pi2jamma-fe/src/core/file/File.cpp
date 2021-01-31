#include "core/file/File.hpp"

#include <sstream>

File::File()
	: mpFile(nullptr)
{
}


File::~File()
{
	close();
}

Result File::open(CStr filePath, OpenMode openMode) {
	close();

	const char* pMode =
		(OpenMode::Read == openMode)
			? "rb"
			: "wb";

	mpFile =
		fopen(
			filePath.c_str(),
			pMode);


	if(nullptr == mpFile) {
		std::stringstream ss;
		ss << "Failed to open file:'" << filePath.c_str() <<"', Mode:'" << pMode << "'.";

		return Result::makeFailureWithString(ss.str());

	}

	return Result::makeSuccess();

}

Result File::readExactly(void* pBuf, FileSize bytes)
{
	auto result = fread(pBuf, 1, bytes, mpFile);
	if(result != bytes) {
		std::stringstream ss;
		ss
			<< "Failed to read exactly: "
			<< bytes
			<< " got: "
			<< result;

		return Result::makeFailureWithString(ss.str());
	}

	return Result::makeSuccess();
}

Result File::getSize(FileSize& size)
{
	auto originalPositionOrError = ftell(mpFile);
	
	if(-1L == originalPositionOrError) {
		return Result::makeFailureWithStringLiteral("ftell() failed");
	}

	if( 0 != fseek(mpFile, 0, SEEK_END)) {
		return Result::makeFailureWithStringLiteral("fseek() failed.");
	}

	auto endPositionOrError = ftell(mpFile);

	if(-1L == endPositionOrError) {
		return Result::makeFailureWithStringLiteral("ftell() failed");
	}	

	if(0 !=fseek(mpFile, 0, originalPositionOrError)) {
		return Result::makeFailureWithStringLiteral("fseek() failed.");
	}

	size = endPositionOrError;

	return Result::makeSuccess();
}

void File::close()
{
	if(nullptr != mpFile) {
		fclose(mpFile);
	}
}
