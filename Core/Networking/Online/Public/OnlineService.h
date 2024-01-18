#pragma once

#ifdef _WIN32
#include <Ws2tcpip.h>
#endif

class UOnlineService
{
public:
	void StartUp();
	void Shutdown();
	void IsInitialized();

private:
	bool bInitializedSuccessfully = false;

#ifdef _WIN32
	WSADATA WSAData;
#endif
};