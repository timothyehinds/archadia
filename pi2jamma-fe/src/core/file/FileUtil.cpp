#include "core/file/FileUtil.hpp"

#include "core/file/File.hpp"

Result<Success> readFile(CStr filename, std::vector<char>& data)
{
	File file;
	Result<Success> r = file.open(filename, File::OpenMode::Read);
	
	if(!r)
	{
		return r;
	}

	Result<FileSize> resultFileSize{file.getSize()};
	if(!resultFileSize)
	{
		return Result<Success>{resultFileSize.moveError()};
	}

	data.resize(*resultFileSize);
	r = file.readExactly(data.data(), *resultFileSize);

	if(!r)
	{
		return r;
	}

	return Result{Success{}};
}