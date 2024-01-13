#pragma once
#include <cstdint>

// https://stackoverflow.com/questions/28027937/cross-platform-sockets
// https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response/22135885#22135885

class USocket
{
public:
	int32_t TestServer();
	int32_t TestClient();
	int32_t Init();
	int32_t Quit();

private:
	static constexpr uint16_t SERVER_PORT = 9909;
	uint64_t  MaxFileDescriptors = 0;
};