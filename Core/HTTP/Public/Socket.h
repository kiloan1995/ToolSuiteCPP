#pragma once
#include <cstdint>
#include <list>
#include <WinSock2.h> // Has to be linked in Solution->Properties->Linker->Input->AdditionalDependencies->wsock32.lib

// https://stackoverflow.com/questions/28027937/cross-platform-sockets
// https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response/22135885#22135885

class USocket
{
public:
	int32_t TestClient();
private:
	static constexpr uint16_t SERVER_PORT = 9909;
	struct sockaddr_in Server;
};

class UServerSocket
{
public:
	int32_t TestServer();
	void ProcessRequest(SOCKET ServerSocket);
	void ProcessMessage(SOCKET ClientSocket);
private:
	static constexpr uint16_t SERVER_PORT = 9909;
	uint64_t  MaxFileDescriptors = 0;
	std::list<SOCKET> Clients;
	struct sockaddr_in Server;
	struct fd_set FD_Readers, FD_Writers, FD_Exceptions; // File descriptors
};