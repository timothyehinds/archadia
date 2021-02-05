
#include "core/CommandLine/CommandLineHandler.hpp"

class CommandLineHandlerConfigFile : public CommandLineHandler
{
public:
	CommandLineHandlerConfigFile()
		: CommandLineHandler( "--config-file", "-c")
		, mConfigFile("./data/config.txt")
	{}

	virtual CStr getHelp() const override {
		return "Loction of the config file.";
	}

	virtual Result<Success> parse(const char**& ppToken, const char** ppEnd) override{
		if(ppToken >= ppEnd) {
			return Result<Success>::makeFailureWithStringLiteral("Expected config file name.");
		}

		mConfigFile = *ppToken;
		ppToken++;

		return Result{Success{}};
	}

	CStr mConfigFile;

	static CommandLineHandlerConfigFile sSingleton;

};
