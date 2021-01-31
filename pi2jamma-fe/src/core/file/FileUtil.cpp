#include "core/file/FileUtil.hpp"

#include "core/file/File.hpp"

Result readFile(CStr filename, std::vector<char>& data)
{
	File file;
	Result r = file.open(filename, File::OpenMode::Read);
	if(r.peekFailed()) {
		return r;
	}

	FileSize fileSize = 0;
	r = file.getSize(fileSize);
	if(r.peekFailed()){
		return r;
	}

	data.resize(fileSize);
	r = file.readExactly(data.data(), fileSize);
	if(r.peekFailed()){
		return r;
	}

	return Result::makeSuccess();
}