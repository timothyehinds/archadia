#pragma once

#include "core/container/RbTree.hpp"
#include "core/CString.hpp"
#include "core/Property.hpp"
#include "core/Result.hpp"

class CommandLineHandler
{
public:
	CommandLineHandler(CStr shortName, CStr longName);
	~CommandLineHandler();

	virtual Result<Success> parse(const char**& ppToken, const char** ppEnd) = 0;
	virtual CStr getHelp() const = 0;

	PROPERTY(CStr, ShortName);
	PROPERTY(CStr, LongName);

public:

	RbTreeNode mShortNameNode;
	RbTreeNode mLongNameNode;
};