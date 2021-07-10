#include <cstdlib>
#include <csignal>
#include <unistd.h>
#include "parser/Config.hpp"
#include "core/Dispatcher.hpp"
#include "utils/Logger.hpp"

static const size_t PAUSE_BEFORE_RESTART = 3 * 1000 * 1000; // N sec

int main(int argc, char *argv[])
{
	Logger::setLogLvl(Logger::NEVER);
	
	if (argc > 2)
	{
		Logger::println("Error: bad arguments. Please, provide path to config or use default.", Logger::ALWAYS);
		return (1);
	}
	const std::string path = argc == 2 ? argv[1] : "config/test.conf";

	for(;;)
	{
		try
		{
			Dispatcher disp(path);
			disp.prep();
			disp.run();
		}
		catch (const Dispatcher::ConfigException &e)
		{
			Logger::println(e.what(), Logger::ALWAYS);
			return 1;
		}
		catch (const Dispatcher::InterruptException &e)
		{
			Logger::println("Bye!!!", Logger::ALWAYS);
			return 0;
		}
		catch (const std::exception &e)
		{
			Logger::println(e.what(), Logger::ALWAYS);
			usleep(PAUSE_BEFORE_RESTART);
		}
	}
	return (0);
}
