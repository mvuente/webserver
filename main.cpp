#include "parser/Config.hpp"
#include "StatusCode.hpp"

int main(int argc, char *argv[])
{
	if (argc > 2)
	{
		std::cout << "Error: bad arguments. Please, provide path to config or use default." << std::endl;
		return (1);
	}
	Config config;
	std::string path;
	path = "config/default.conf";
	if (argc == 2)
		path = argv[1];
	config.parse(path);
	return (0);
}