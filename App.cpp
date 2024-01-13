// ToolSuiteCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Core/Core.h"

/**
https://stackoverflow.com/questions/45885746/avoid-implementation-defined-types-in-platform-independent-code
allowed types
'char' for characters
'int32_t' ... 'intx_t'
'uint32_t' ...'uintx_t'
https://stackoverflow.com/questions/30706230/is-c-stdstring-platform-independent
'std::string' should be platform independent
*/
int main()
{
	UCore Core = UCore();
	return 0;
}


