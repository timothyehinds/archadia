#include "system.hpp"

#include <memory>

Result<Success> System::exec(const char* cmd, std::string& result)
{

	std::array<char,128> buffer;
	std::unique_ptr<FILE,int(*)(FILE*)> pipe(popen(cmd, "r"), &pclose);

	if(!pipe) {
		return Result<Success>::makeFailureWithStringLiteral("Failed to open pipe");;
	}

	while(!feof(pipe.get())) {
		if(fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr ) {
				result += buffer.data();
		}
	}

	return Result{Success{}};
}