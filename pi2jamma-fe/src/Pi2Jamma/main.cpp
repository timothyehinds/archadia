#include "Pi2JammaApplication.hpp"

int main(int argc, const char* argv[])
{
	Pi2JammaApplication application;

	Result result = application.run(argc, argv);
	if(!result) {
		Log(result.getMessage());
		return -1;
	}

	return 0;
}