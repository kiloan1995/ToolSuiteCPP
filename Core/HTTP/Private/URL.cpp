#include <string>
#include "../Public/URL.h"
#include "../../Insights/Public/Logger.h"
#include <iostream>

FURL::FURL(const std::string& Link)
{
	if (Link.length() == 0) return;

	size_t Pos = Link.find("://");
	if (Pos > -1) // At first we can find the scheme
	{
		Scheme = Link.substr(0, Pos);
	}
	else
	{
		Pos = 0;
	}

	std::vector<size_t> positions;
	std::string Sub = "/";
	size_t SegPos = Link.find(Sub, Pos);
	while (SegPos != std::string::npos)
	{
		positions.push_back(SegPos);
		SegPos = Link.find(Sub, SegPos + 1);
		ULogger::Log(SegPos);
	}
}