#pragma once

#include "core/CString.hpp"
#include "core/Result.hpp"

#include <stdio.h>

using FileSize = uint64_t;

class File
{
public:

	enum class OpenMode { Read, Write };

	File();
	~File();

	Result open(CStr filePath, OpenMode openMOde);
	Result readExactly(void* pBuf, FileSize bytes);
	Result getSize(FileSize& size);

	void close();

private:

	FILE* mpFile;
};
