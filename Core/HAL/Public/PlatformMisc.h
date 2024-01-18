#pragma once
#include <string>

class FPlatformMisc
{
public:
	static bool ClipBoardCopy(const std::string& String);
	static bool ClipBoardPaste(std::string& String);
};
