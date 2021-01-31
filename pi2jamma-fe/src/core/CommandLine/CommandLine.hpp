#pragma once

#include "core/CommandLine/CommandLineHandler.hpp"

class CommandLine
{
public:
	static CommandLine& get();
	void addHandler(CommandLineHandler& handler);
	void removeHandler(CommandLineHandler& handler);

	Result parse(int argc, const char* argv[]);
	void printHelp() const;

private:

	CStr mApplicationPath;

	RbTree<
		CommandLineHandler,
		CStr,
		KeyFinderGetter<
			CommandLineHandler,
			CStr,
			& CommandLineHandler::getShortName>,
		NodeFinderField<
			CommandLineHandler,
			RbTreeNode,
			& CommandLineHandler::mShortNameNode>> mHandlersByShortName;

	RbTree<
		CommandLineHandler,
		CStr,
		KeyFinderGetter<
			CommandLineHandler,
			CStr,
			& CommandLineHandler::getLongName>,
		NodeFinderField<
			CommandLineHandler,
			RbTreeNode,
			& CommandLineHandler::mLongNameNode>> mHandlersByLongName;
};

