#pragma once
#include <cstdint>
#include <string>

// https://stackoverflow.com/questions/28027937/cross-platform-sockets
// https://stackoverflow.com/questions/22077802/simple-c-example-of-doing-an-http-post-and-consuming-the-response/22135885#22135885

struct FURL;

class USocket
{
public:
	bool Connect(const FURL* Url);
	bool Receive(std::string& Result) const;
	bool Send(const std::string& Request) const;

private:
	bool Create();
	void Close() const;
	unsigned long long Socket = 0;

};

