#pragma once
#include <iostream>

enum class ELogLevel : uint8_t
{
	LOG_NORMAL,
	LOG_WARNING,
	LOG_ERROR,
	LoggerLevel_MAX
};


class ULogger
{
public:
	// Specify the Loglevel;
	template<typename First, typename ... Strings>
	static void Log(ELogLevel LogLevel, First arg, const Strings&... rest)
	{
		if (!CanLog(LogLevel)) return;

		SetConsoleColor(LogLevel);

		std::cout << arg << " ";
		ULogger::Log(rest...);
	}

	// Default LoggerLevel is LOG_NORMAL;
	template<typename First, typename ... Strings>
	static void Log(First arg, const Strings&... rest)
	{
		ULogger::Log(ELogLevel::LOG_NORMAL, arg, rest...);
	}

private:
	static void SetConsoleColor(ELogLevel LogLevel);
	static int GetConsoleColor(ELogLevel LogLevel);

	static void Log()
	{
		std::cout << std::endl;
	}

	static bool CanLog(ELogLevel LogLevel)
	{
		return (LogLevelFlags >> (uint8_t)LogLevel) & 1;
	}

	static void SetLogLevelEnabled(ELogLevel LogLevel)
	{
		LogLevelFlags |= 1 << static_cast<uint8_t>(LogLevel);
	}

	static void SetLogLevelDisabled(ELogLevel LogLevel)
	{
		LogLevelFlags &= ~(1 << static_cast<uint8_t>(LogLevel));
	}
	static uint32_t LogLevelFlags;

};

