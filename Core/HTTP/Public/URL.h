#pragma once
#include <string>
#include <vector>

struct FURL
{
public:
	FURL(const std::string& Link);

private:
	std::string Scheme;
	std::string User;
	std::string Password;
	std::string Host;
	std::string Port;
	std::string Path;
	std::vector<std::string> Query;
	std::string Fragment;
};