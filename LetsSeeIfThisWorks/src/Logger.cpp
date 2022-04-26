#include "Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Logger::logger;

void Logger::Init()
{

	logger = spdlog::stdout_color_mt("LOG");
	logger->set_level(spdlog::level::trace);

}

