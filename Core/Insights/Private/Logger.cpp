#include "../Public/Logger.h"
#include <WinSock2.h>

uint32_t ULogger::LogLevelFlags = 0xFFFFFFFF;

int ULogger::GetConsoleColor(ELogLevel LogLevel)
{
	// https://gist.github.com/Kielx/2917687bc30f567d45e15a4577772b02
/** LINUX
 *	#define RESET   "\033[0m"
 *	#define BLACK   "\033[30m"
 *	#define RED     "\033[31m"
 *	#define GREEN   "\033[32m"
 *	#define YELLOW  "\033[33m"
 *	#define BLUE    "\033[34m"
 *	#define MAGENTA "\033[35m"
 *	#define CYAN    "\033[36m"
 *	#define WHITE   "\033[37m"
 *	#define BOLDBLACK   "\033[1m\033[30m"
 *	#define BOLDRED     "\033[1m\033[31m"
 *	#define BOLDGREEN   "\033[1m\033[32m"
 *	#define BOLDYELLOW  "\033[1m\033[33m"
 *	#define BOLDBLUE    "\033[1m\033[34m"
 *	#define BOLDMAGENTA "\033[1m\033[35m"
 *	#define BOLDCYAN    "\033[1m\033[36m"
 *	#define BOLDWHITE   "\033[1m\033[37m"
 **/

	// https://learn.microsoft.com/en-us/windows/console/console-screen-buffers#span-idwin32characterattributesspanspan-idwin32characterattributesspancharacter-attributes
	switch (LogLevel)
	{
		default:
		case ELogLevel::LOG_NORMAL: return 7;
		case ELogLevel::LOG_WARNING: return 14;
		case ELogLevel::LOG_ERROR: return FOREGROUND_RED;
	}
}

void ULogger::SetConsoleColor(ELogLevel LogLevel)
{
	HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
	int ColorID = GetConsoleColor(ELogLevel::LOG_NORMAL);
	SetConsoleTextAttribute(Console, ColorID);
}