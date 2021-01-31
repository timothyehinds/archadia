#include "core/CommandLine/CommandLineHandler.hpp"

#include "core/CommandLine/CommandLine.hpp"

CommandLineHandler::CommandLineHandler(CStr shortName, CStr longName)
	: mShortName(shortName)
	, mLongName(longName)
{
	CommandLine::get().addHandler(*this);	
}

CommandLineHandler::~CommandLineHandler()
{
	CommandLine::get().removeHandler(*this);
}
