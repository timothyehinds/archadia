#include "Pi2JammaApplication.hpp"

#include "core/system.hpp"

int main(int argc, const char* argv[])
{
	for (size_t i{0}; i < 3; i ++)
	{
		{
			Pi2JammaApplication application;

			Result result = application.run(argc, argv);
			if(!result) {
				Log(result.getMessage());
				return -1;
			}
		}

		std::string retroarchOutput;

		Result<Success> retroarchResult{System::exec("retroarch -c ./data/retroarch.config", retroarchOutput)};

	}

	return 0;
}