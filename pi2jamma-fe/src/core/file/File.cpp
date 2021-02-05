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

Result<Success> File::open(const CStr filePath, const OpenMode openMode)
{
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

		return Result<Success>::makeFailureWithString(ss.str());

	}

	return Result{Success{}};

}

Result<Success> File::readExactly(void* pBuf, FileSize bytes)
{
	auto result = fread(pBuf, 1, bytes, mpFile);
	if(result != bytes) {
		std::stringstream ss;
		ss
			<< "Failed to read exactly: "
			<< bytes
			<< " got: "
			<< result;

		return Result<Success>::makeFailureWithString(ss.str());
	}

	return Result{Success{}};
}

Result<FileSize> File::getSize()
{
	auto originalPositionOrError = ftell(mpFile);
	
	if(-1L == originalPositionOrError) {
		return Result<FileSize>::makeFailureWithStringLiteral("ftell() failed");
	}

	if( 0 != fseek(mpFile, 0, SEEK_END)) {
		return Result<FileSize>::makeFailureWithStringLiteral("fseek() failed.");
	}

	auto endPositionOrError = ftell(mpFile);

	if(-1L == endPositionOrError) {
		return Result<FileSize>::makeFailureWithStringLiteral("ftell() failed");
	}	

	if(0 !=fseek(mpFile, 0, originalPositionOrError)) {
		return Result<FileSize>::makeFailureWithStringLiteral("fseek() failed.");
	}

	return Result<FileSize>{static_cast<FileSize>(endPositionOrError)};
}

void File::close()
{
	if(nullptr != mpFile) {
		fclose(mpFile);
	}
}
