

#include <WinSock2.h> // Has to be linked in Solution->Properties->Linker->Input->AdditionalDependencies->wsock32.lib
#include <Ws2tcpip.h>
#include "../public/Socket.h"
#include "../../Insights/Public/Logger.h"
#include <iostream>

struct sockaddr_in Server;
struct fd_set FD_Readers, FD_Writers, FD_Exceptions; // File descriptors

int32_t USocket::TestServer()
{
	// https://www.youtube.com/watch?v=W9b9SaGXIjA&list=PLhnN2F9NiVmAMn9iGB_Rtjs3aGef3GpSm&index=2&ab_channel=VartetaLearningPlatform
	// Remember to always call 		WSACleanup(); whenever we exit this function, stop using the socket.
	int Result = EXIT_SUCCESS;
	// Initialize the WSA variables (when using windows)
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
	{
		ULogger::Log(ELogLevel::LOG_ERROR, "WSAStartup failed");
		WSACleanup();
		return EXIT_FAILURE;
	}
	ULogger::Log("WSAStartup success");

	// Initialize Socket

	// 1.arg ->AF_INET is for UDP, TCP, etc.
	// 3. arg -> AF_UNSPEC, can be other protocoll like TCP, UDP, etc.
	const SOCKET Socket = socket(AF_INET, SOCK_STREAM, AF_UNSPEC);
	if (Socket < 1)
	{
		ULogger::Log(ELogLevel::LOG_ERROR, "Socket call failed");
		WSACleanup();
		return EXIT_FAILURE;
	}
	ULogger::Log("Socket", Socket, "call success");

	// Initialize the environment for sockaddr structure

	Server.sin_family = AF_INET;
	Server.sin_port = htons(SERVER_PORT);
	Server.sin_addr.s_addr = INADDR_ANY; // Here we are assigning the address of the local machine (Own System IP Adress)
	//Server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Alternative. Will convert to the correct byteorder
	memset(&(Server.sin_zero), 0, 8);

	int OptionalVal = 0;
	int OptionalValSize = sizeof(OptionalVal);
	Result = setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&OptionalVal, OptionalValSize);

	if (!Result)
	{
		ULogger::Log("setsockopt call successfull");
	}
	else
	{
		ULogger::Log(ELogLevel::LOG_ERROR, "setsockopt call failed");
		WSACleanup();
		return EXIT_FAILURE;
	}

	// Blocking vs Non Blocking sockets
	u_long IsBlocking = 0; // 0 means blocking, != 0 means non-blocking;
	Result = ioctlsocket(Socket, FIONBIO, &IsBlocking); // every socket is default blocking, so we call here to changed that if argp != 0;
	if (Result != 0)
	{
		ULogger::Log(ELogLevel::LOG_ERROR, "ioctlsocket call failed");
	}
	else
	{
		ULogger::Log("ioctlsocket call passed");
	}

	// Bind the socket tot the local port
	Result = bind(Socket, (sockaddr*)&Server, sizeof(sockaddr));
	if (Result < 0)
	{
		ULogger::Log(ELogLevel::LOG_ERROR, "Failed to bind to locale port");
		WSACleanup();
		return EXIT_FAILURE;
	}
	ULogger::Log("Successfully bound to local port");

	// Listen the request from client (queues the requests)
	Result = listen(Socket, 5); // backlog says how many requests at a time the Server can pull (how many requests can be in the active queue).
	if (Result < 0)
	{
		ULogger::Log(ELogLevel::LOG_ERROR, "Failed to start 'listen to local port'");
		WSACleanup();
		return EXIT_FAILURE;
	}
	ULogger::Log("Started listening to local port");

	//int32_t N = 100;
//for (int32_t i = 0; i < 64; ++i)
//{
//	FD_SET(N, &FD_Readers);
//	++N;
//}

//ULogger::Log(FD_Readers.fd_count);

//for (int32_t i = 0; i < 64; ++i)
//{
//	ULogger::Log(FD_Readers.fd_array[i]);
//}

	MaxFileDescriptors = Socket;

	struct timeval TimeValue;
	TimeValue.tv_sec = 1; // Time we wait to see wether any of the sockets are ready or not.
	TimeValue.tv_usec = 0; // 

	while (true)
	{

		// First we clear all of them.
		FD_ZERO(&FD_Readers);
		FD_ZERO(&FD_Writers);
		FD_ZERO(&FD_Exceptions);

		FD_SET(Socket, &FD_Readers);
		FD_SET(Socket, &FD_Exceptions);

		ULogger::Log("Before select call:", FD_Readers.fd_count);

		// Keep waiting for new requests and process as per the request
		Result = select(MaxFileDescriptors + 1, &FD_Readers, &FD_Writers, &FD_Exceptions, &TimeValue);

		if (Result > 0) // When someone connects or communicates with a message over a dedicated connection
		{
			ULogger::Log("Data on port", SERVER_PORT, ", Processing now");
			if (FD_ISSET(Socket, &FD_Exceptions))
			{
				ULogger::Log("There is an exception. Just get away from here.");
			}

			if (FD_ISSET(Socket, &FD_Writers))
			{
				ULogger::Log("Ready to write something.");
			}

			if (FD_ISSET(Socket, &FD_Readers))
			{
				ULogger::Log("Ready to read something. Something new came up at the port", SERVER_PORT);
			}
		}
		else if (Result == 0) // No connection or any communication request made OR non of the descriptors are ready
		{
			ULogger::Log("Nothing on Port", SERVER_PORT);
		}
		else // error, show some useful message
		{
			WSACleanup();
			return EXIT_FAILURE;
		}
		ULogger::Log("After select call:", FD_Readers.fd_count);
	}
	WSACleanup();
	return EXIT_SUCCESS;
}

int32_t USocket::TestClient()
{
	int Result = EXIT_SUCCESS;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
	{
		ULogger::Log(ELogLevel::LOG_ERROR, "WSAStartup failed");
		WSACleanup();
		return EXIT_FAILURE;
	}
	ULogger::Log("WSAStartup success");

	// Initialize Socket

	const SOCKET Socket = socket(AF_INET, SOCK_STREAM, AF_UNSPEC);
	if (Socket < 1)
	{
		ULogger::Log(ELogLevel::LOG_ERROR, "Socket call failed");
		WSACleanup();
		return EXIT_FAILURE;
	}
	ULogger::Log("Socket", Socket, "call success");

	// Initialize the environment for sockaddr structure

	Server.sin_family = AF_INET;
	Server.sin_port = htons(SERVER_PORT);
	//Server.sin_addr.s_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, "127.0.0.1", &(Server.sin_addr));
	memset(&(Server.sin_zero), 0, 8);

	Result = connect(Socket, (struct sockaddr*)&Server, sizeof(Server));
	if (Result < 0)
	{
		ULogger::Log(ELogLevel::LOG_ERROR, "connect failed");
		WSACleanup();
		return EXIT_FAILURE;
	}
	ULogger::Log("Connected to Server");
}

int32_t USocket::Init()
{
	return 0;
}

int32_t USocket::Quit()
{
	return 0;
}