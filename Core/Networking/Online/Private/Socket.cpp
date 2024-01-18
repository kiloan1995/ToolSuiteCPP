
#ifdef _WIN32
#include <WinSock2.h> // Has to be linked in Solution->Properties->Linker->Input->AdditionalDependencies->wsock32.lib
#include <Ws2tcpip.h>
#endif
#include "../Public/Socket.h"
#include "../Public/URL.h"
#include "../../../Insights/Public/Logger.h"

void USocket::Close() const
{
#ifdef _WIN32
	ULogger::Log("Socket Closed");
	const int Result = closesocket(Socket);
#endif
}

bool USocket::Connect(const FURL* Url)
{
#ifdef _WIN32
	// https://cppsecrets.com/users/18989711511997116104103495564103109971051084699111109/C00-Socket-Programming-getaddrinfo.php
	// https://gist.github.com/Ben-0-mad/5abd651dbf8ef2e9f32b254463156bf2

	//sockaddr_in hint;
	//hint.sin_family = AF_INET;
	////hint.sin_port = htons(Url->Port.length() > 0 ? std::stoi(Url->Port) : Port);
	//hint.sin_port = htons(80);
	//inet_pton(AF_INET, (Url->Host + ":" + Url->Port + Url->Path).c_str(), &hint.sin_addr);

	//// connect
	//if (connect(Socket, (sockaddr*)&hint, sizeof(hint)) != 0)
	//{
	//	ULogger::Log("Error: Can't connect to server");
	//	Close();
	//	return false;
	//}
	//return true;

	// http://www.zedwood.com/article/cpp-winsock-basic-http-connection

	//std::string request;
	//request += "GET /test.html HTTP/1.0\r\n";
	//request += "Host: www.zedwood.com\r\n";
	//request += "\r\n";
	//serveraddr.sin_addr.s_addr = inet_addr(ipaddress);
	addrinfo* results;
	addrinfo hints;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo("ww.google.com", "domain", &hints, &results) != 0)
	{
		ULogger::Log("Error: getaddrinfo failed");
	}

	uint16_t Port = 0;


	switch (results->ai_family)
	{
		// https://stackoverflow.com/questions/39434134/why-does-getaddrinfo-return-a-port-number-of-20480-for-http
		// https://stackoverflow.com/questions/46679145/why-inet-ntop-and-inet-ntoa-gives-different-result
		// https://stackoverflow.com/questions/38848281/inet-ntop-printing-incorrect-ipv6-address
		// 1. we convert  to the correct type (is in data[])
		// 2. We have to convert he byteorder
		// 3. We read the IPAdress as string;
		// 4. We convert the string to the serveraddres;
		default:break;
		case AF_INET:
		{
			sockaddr_in serveraddr = {};
			memset(&serveraddr, 0, sizeof(serveraddr));
			serveraddr.sin_family = results->ai_family;
			const sockaddr_in* addr = (sockaddr_in*)results->ai_addr;
			Port = ntohs(addr->sin_port);
			serveraddr.sin_port = htons(Port);
			char IPString[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &(addr->sin_addr), IPString, INET_ADDRSTRLEN);
			inet_pton(AF_INET, IPString, &serveraddr.sin_addr.s_addr);
			if (connect(Socket, (const sockaddr*)&serveraddr, sizeof(serveraddr) != 0))
			{
				ULogger::Log("Error: Can't connect to server");
				Close();
				return false;
			}
			break;
		}
		case AF_INET6:
		{
			sockaddr_in6 serveraddr = {};
			memset(&serveraddr, 0, sizeof(sockaddr_in6));

			serveraddr.sin6_family = AF_INET6;
			const sockaddr_in6* addr = (sockaddr_in6*)results->ai_addr;
			Port = ntohs(addr->sin6_port);
			serveraddr.sin6_port = htons(Port);
			char IPString[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6, &(addr->sin6_addr), IPString, INET6_ADDRSTRLEN);
			inet_pton(AF_INET6, IPString, &serveraddr.sin6_addr);

			SOCKET NewSocket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

			if (connect(NewSocket, (const sockaddr*)&serveraddr, sizeof(sockaddr_in6)) == SOCKET_ERROR)
			{
				ULogger::Log("Error: Can't connect to server with IP:", IPString, "Port:", Port);
				Close();
				return false;
			}
			break;
		}
	}

	freeaddrinfo(results);

	Close();
	return true;
#endif
	Close();
	return false;
}

bool USocket::Create()
{
#ifdef _WIN32
	Socket = socket(AF_INET, SOCK_STREAM, 0);
	//Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	return Socket != INVALID_SOCKET;
#endif
	return false;
}

bool USocket::Receive(std::string& Result) const
{
#ifdef _WIN32
	if (Result.length() > 0)
	{
		Result = "";
	}

	char buffer[10000];
	int nDataLength;
	while ((nDataLength = recv(Socket, buffer, 10000, 0)) > 0) {
		int i = 0;
		while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {

			Result += buffer[i];
			i += 1;
		}
	}
	Close();
	return true;
#endif
	Close();
	return false;
}

bool USocket::Send(const std::string& Request) const
{
#ifdef _WIN32
	const int Result = send(Socket, Request.c_str(), strlen(Request.c_str()), 0);
	if (Result != 0)
	{
		Close();
		return false;
	}
	return true;
#endif
	return false;
}
