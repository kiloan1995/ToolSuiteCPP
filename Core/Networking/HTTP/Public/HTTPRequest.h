#pragma once
#include <string>

struct FURL;
enum class EHTTPRequestMethod
{
	METHOD_GET,		// Get contents from server
	METHOD_POST,		// Send data to Server
	METHOD_HEAD,		// Make Server send HTTP_Header without content
	METHOD_PUT,		// Update on a resource
	METHOD_DELETE,		// Deletes file on server. Often not implemented (security reasons)
	METHOD_TRACE,		// Feeds back to server for debugging purposes. Often not implemented (security reasons)
	METHOD_CONNECT,	// Implemented by proxies
	METHOD_OPTIONS,	// 
	METHOD_PATCH,		// Change of a resource
	EHTTPRequestMethod_MAX
};

enum class EHTTPVersion
{
	VERSION_HTTP_1,
	VERSION_HTTP_1_1,
	VERSION_HTTP_2,
	VERSION_HTTP_3,
	EHTTPVersion_MAX
};

class UHTTPRequest
{
public:
	bool Process(const FURL* Url) const;


private:
	EHTTPRequestMethod Method = EHTTPRequestMethod::METHOD_GET;
	EHTTPVersion Version = EHTTPVersion::VERSION_HTTP_2;
	std::string Header;
	std::string Url;
	std::string Content;
	uint16_t Port = 80;
	static std::string GetMethod(EHTTPRequestMethod Method);
	static std::string GetVersion(EHTTPVersion Version);
};