#pragma once

#include "core/CommandLine/CommandLineHandler.hpp"

class CommandLineHandlerHelp : public CommandLineHandler
{
public:
	CommandLineHandlerHelp();
	virtual Result<Success> parse(const char**& pS, const char** pEnd) override;
	virtual CStr getHelp() const override;
private:
	static CommandLineHandlerHelp sSingleton;

};
