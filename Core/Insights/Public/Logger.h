#pragma once
#include <string>
#include <iostream>

class ULogger
{
public:


	template<typename First, typename ... Strings>
	static void Log(First arg, const Strings&... rest)
	{
		std::cout << arg << " ";
		ULogger::Log(rest...);
	};

private:
	static void Log()
	{
		std::cout << std::endl;
	};
};
