#pragma once
#include <regex>
#include <string>
#include <vector>

struct FURL
{
public:
	FURL(const std::string& Link);

private:
	void Parse(const std::string& Url, bool bSlashesDenoteHost, bool bParseQueryString);
	void ToLower(const std::string& String);
	static std::smatch Exec(const std::string& String, const std::regex& Regex);
	static bool Test(const std::string& String, const std::regex& Regex);
	static bool IsIpv6HostName(const std::string& HostName);
	std::string GetHostName(const std::string& Rest, const std::string HostName&, const std::string& Url);
	std::string Scheme; //altname: Protocoll
	std::string User;
	std::string Password;
	std::string Host;
	std::string Port;
	std::string Path;
	std::string Query;
	std::string Fragment;
};