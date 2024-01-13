#pragma once
#include <cstdint>

// https://stackoverflow.com/questions/28027937/cross-platform-sockets
// https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response/22135885#22135885

class USocket
{
	int32_t Init();
	int32_t Quit();
};