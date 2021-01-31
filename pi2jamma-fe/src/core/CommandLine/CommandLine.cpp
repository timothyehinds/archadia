#include "core/CommandLine/CommandLine.hpp"

#include "core/cast.hpp"
#include "core/StringUtil.hpp"
#include "core/file/FilePath.hpp"

CommandLine& CommandLine::get()
{
	static CommandLine sCommandLine;
	return sCommandLine;
}

Result CommandLine::parse(int argc, const char* argv[])
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
					Result::makeFailureWithString(
						formatString(
							"unrecognized argument: '%s'",
							*ppCurrent ));
			}
		}

		ppCurrent++;

		Result r = pH->parse(ppCurrent, ppEnd);
		if (r.peekFailed()) {
			printHelp();
			return r;
		}
	}

	return Result::makeSuccess();
}

void CommandLine::printHelp() const
{
	fprintf(
		stderr,
		"%s Help:\n",
		getPathEntryName(mApplicationPath).c_str());

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
