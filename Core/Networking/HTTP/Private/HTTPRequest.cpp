#include "../Public/HTTPRequest.h"
#include "../../Online/Public/URL.h"
#include "../../Online/Public/Socket.h"
#include "../../../Insights/Public/Logger.h"

#ifdef __linux__ 
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#elif _WIN32
#include <locale>

#endif



bool UHTTPRequest::Process(const FURL* Url) const
{
#ifdef __linux__  // https://stackoverflow.com/questions/70550983/http-request-using-sockets-on-c
	int socket_desc;
	struct sockaddr_in serv_addr;
	struct hostent* server;
	char buffer[4096];

	std::string host = "www.exampleweb.com";
	std::string port = "80";
	std::string resource = "/folder/file.php";
	std::string query = "?parameter=value&parameter2=value2";

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc < 0) {
		ULooger::Log("Error: failed to create socket");
		return false;
	}

	server = gethostbyname(host.c_str());
	if (server == NULL) {
		ULooger::Log("Error: could Not resolve hostname");
		close(socket_desc);
		return false;
	}

	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(std::stoi(port));
	bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);

	if (connect(socket_desc, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		ULooger::Log("Error: could connection failed");
		close(socket_desc);
		return false;
	}

	std::string request = "GET " + resource + query + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";

	if (send(socket_desc, request.c_str(), request.size(), 0) < 0) {
		ULooger::Log("Error: failed to send request...");
		close(socket_desc);
		return false;
	}

	int n;
	std::string raw_site;
	while ((n = recv(socket_desc, buffer, sizeof(buffer), 0)) > 0) {
		raw_site.append(buffer, n);
	}

	close(socket_desc);

	ULooger::Log(raw_site);
	return false;

#elif _WIN32 // https://stackoverflow.com/questions/39931347/simple-http-get-with-winsock

	USocket Socket;
	if (!Socket.Connect(Url)) return false;

	const std::string Request = "Hello";
	if (!Socket.Send(Request)) return false;

	std::string Result;
	if (!Socket.Receive(Result)) return false;
	
	return true;
#endif
	return false;
}

std::string UHTTPRequest::GetMethod(EHTTPRequestMethod Method)
{
	switch (Method)
	{
		default:
		case EHTTPRequestMethod::METHOD_GET: return "GET";
		case EHTTPRequestMethod::METHOD_POST:return "POST";
		case EHTTPRequestMethod::METHOD_HEAD:return "HEAD";
		case EHTTPRequestMethod::METHOD_PUT:return "PUT";
		case EHTTPRequestMethod::METHOD_DELETE:return "DELETE";
		case EHTTPRequestMethod::METHOD_TRACE:return "TRACE";
		case EHTTPRequestMethod::METHOD_CONNECT:return "CONNECT";
		case EHTTPRequestMethod::METHOD_OPTIONS:return "OPTIONS";
		case EHTTPRequestMethod::METHOD_PATCH:return "PATCH";
	}
}

std::string UHTTPRequest::GetVersion(EHTTPVersion Version)
{
	switch (Version)
	{
		case EHTTPVersion::VERSION_HTTP_1: return "HTTP/1";
		case EHTTPVersion::VERSION_HTTP_1_1: return "HTTP/1.1";
		default:
		case EHTTPVersion::VERSION_HTTP_2: return "HTTP/2";
		case EHTTPVersion::VERSION_HTTP_3: return "HTTP/3";
	}
}