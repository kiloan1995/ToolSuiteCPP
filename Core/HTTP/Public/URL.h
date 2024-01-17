#pragma once
#include <regex>
#include <string>
#include <vector>

struct FURL
{
public:
	FURL(const std::string& Link);
	std::string GetHref();

private:
	void Init();
	void Parse(const std::string& Url, bool bSlashesDenoteHost, bool bParseQueryString);
	void ToLower(std::string& String);
	std::vector<std::string> Exec(const std::string& String, std::regex Regex);
	static bool Test(const std::string& String, std::regex Regex);
	static bool IsIpv6HostName(const std::string& HostName);
	std::string GetHostName(const std::string& Rest, const std::string& HostName);
	std::string AutoEscapeAllDelimiters(const std::string& String);

	std::string Scheme; //altname: Protocoll
	std::string User;
	std::string Password;
	std::string Host;
	std::string Port;
	std::string Path;
	std::string Query;
	std::string Fragment;

	std::regex HostPattern;
	std::regex PortPattern;
	std::regex ProtocolPattern; // Scheme

	std::regex SimplePathPattern;
	std::regex ForbiddenHostChars;
	std::regex ForbiddenHostCharsIpv6;
};