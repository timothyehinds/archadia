#include "core/CommandLine/CommandLine.hpp"

#include "core/cast.hpp"
#include "core/StringUtil.hpp"

#include <filesystem>

namespace fs = std::filesystem;

CommandLine& CommandLine::get()
{
	static CommandLine sCommandLine;
	return sCommandLine;
}

Result<Success> CommandLine::parse(int argc, const char* argv[])
{
	mApplicationPath = argv[0];

	const char** ppStart = & argv[1];
	const char** ppEnd = & argv[argc];	

	const char** ppCurrent = ppStart;

	while(ppCurrent < ppEnd) {
		Log("Testy\n");
		CommandLineHandler* pH = mHandlersByShortName.find(*ppCurrent);
		if(nullptr == pH) {
			pH = mHandlersByLongName.find(*ppCurrent);
	
			if(nullptr == pH) {
				printHelp();
				return
					Result<Success>::makeFailureWithString(
						formatString(
							"unrecognized argument: '%s'",
							*ppCurrent ));
			}
		}

		ppCurrent++;

		Result<Success> r = pH->parse(ppCurrent, ppEnd);
		if (!r) {
			printHelp();
			return r;
		}
	}

	return Result{Success{}};
}

void CommandLine::printHelp() const
{
	fprintf(
		stderr,
		"%s Help:\n",
		fs::path(mApplicationPath.c_str()).filename().c_str());

	for(auto&& h : mHandlersByLongName)
	{
		fprintf(
			stderr,
			"\t%s, %s - %s\n",
			h.getLongName().c_str(),
			h.getShortName().c_str(),
			h.getHelp().c_str());
	}
}

void CommandLine::addHandler(CommandLineHandler& handler)
{
	mHandlersByShortName.insert(handler);
	mHandlersByLongName.insert(handler);
}

void CommandLine::removeHandler(CommandLineHandler& handler)
{
	mHandlersByShortName.remove(handler);
	mHandlersByLongName.remove(handler);
}
