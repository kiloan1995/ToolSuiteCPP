

#include <winsock.h> // Has to be linked in Solution->Properties->Linker->Input->AdditionalDependencies->wsock32.lib
#include "../public/Socket.h"
#include "../../Insights/Public/Logger.h"

#include <iostream>

struct sockaddr_in Server;
struct fd_set FD_Readers, FD_Writers, FD_Exceptions; // File descriptors

int32_t USocket::TestServer()
{
	// https://www.youtube.com/watch?v=W9b9SaGXIjA&list=PLhnN2F9NiVmAMn9iGB_Rtjs3aGef3GpSm&index=2&ab_channel=VartetaLearningPlatform

	int Result = EXIT_SUCCESS;
	// Initialize the WSA variables (when using windows)
	WSADATA wsa;
	if (const int Result = WSAStartup(MAKEWORD(2, 2), &wsa) > -1)
	{
		ULogger::Log("WSAStartup", Result, "started successfully");
	}
	else
	{
		ULogger::Log("WSAStartup failed");
	}

	// Initialize Socket

	// 1.arg ->AF_INET is for UDP, TCP, etc.
	// 3. arg -> AF_UNSPEC, can be other protocoll like TCP, UDP, etc.
	const SOCKET Socket = socket(AF_INET, SOCK_STREAM, AF_UNSPEC);
	if (Socket < 1)
	{
		ULogger::Log("Socket not opened!");
	}
	else
	{
		ULogger::Log("Socket", Socket, "opened successfully");
	}
	// Initialize the environment for sockaddr structure

	Server.sin_family = AF_INET;
	Server.sin_port = htons(SERVER_PORT);
	Server.sin_addr.s_addr = INADDR_ANY; // Here we are assigning the address of the local machine (Own System IP Adress)
	//Server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Alternative. Will convert to the correct byteorder
	memset(&(Server.sin_zero), 0, 8);


	// Bind the socket tot the local port
	Result = bind(Socket, (sockaddr*)&Server, sizeof(sockaddr));
	if (Result < 0)
	{
		ULogger::Log("Failed to bind to locale port");
		return EXIT_FAILURE;
	}
	else
	{
		ULogger::Log("Successfully bound to local port");
	}
	// Listen the request from client (queues the requests)
	Result = listen(Socket, 5); // backlog says how many requests at a time the Server can pull (how many requests can be in the active queue).
	if (Result < 0)
	{
		ULogger::Log("Failed to start 'listen to local port'");
		return EXIT_FAILURE;
	}
	else
	{
		ULogger::Log("Started listening to local port");
	}
	// Keep waiting for new requests and process as per the request
	return EXIT_SUCCESS;
}

int32_t USocket::Init()
{
	return 0;
}

int32_t USocket::Quit()
{
	return 0;
}