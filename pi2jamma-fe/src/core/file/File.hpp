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

	[[nodiscard]] Result<Success> open(CStr filePath, OpenMode);
	[[nodiscard]] Result<Success> readExactly(void* pBuf, FileSize bytes);
	[[nodiscard]] Result<FileSize> getSize();

	void close();

private:

	FILE* mpFile;
};
