#pragma once

#include "spdlog/spdlog.h"

class Logger
{
public:

	Logger() = delete;

	static void Init();
	static std::shared_ptr<spdlog::logger>& GetLogger() { return logger; }

private:

	static std::shared_ptr<spdlog::logger> logger;

};