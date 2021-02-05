
#include "core/CommandLine/CommandLineHandlerHelp.hpp"

#include "core/CommandLine/CommandLine.hpp"

CommandLineHandlerHelp CommandLineHandlerHelp::sSingleton;

CommandLineHandlerHelp::CommandLineHandlerHelp()
	: CommandLineHandler("--help", "-h")
{}

Result<Success> CommandLineHandlerHelp::parse(const char**& pS, const char** pEnd)
{
	CommandLine::get().printHelp();
	return Result{Success{}};
}

CStr CommandLineHandlerHelp::getHelp() const {
	return "Print help.";
}