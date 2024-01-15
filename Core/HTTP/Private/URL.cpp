#include <string>
#include "../Public/URL.h"
#include <iostream>
#include <regex>
#include <set>

// Node.js source code
// Reference: RFC 3986, RFC 1808, RFC 2396
static const std::regex ProtocolPattern("/^[a-z0-9.+-]+:/i");
static const std::regex PortPattern("/:[0-9]*$/");
static const std::regex HostPattern("/^\/\/[^@/]+@[^@/]+/");
// Special case for a simple path URL
static const std::regex SimplePathPattern("/^(\/\/?(?!\/)[^?\s]*)(\?[^\s]*)?$/");
static constexpr int HostNameMaxLength = 255;
static constexpr std::set<std::string> HostlessProtocol = { "javascript", "javascript:" };
static constexpr std::set<std::string> SlashedProtocol = { "http", "http:",  "https",  "https:",  "ftp",  "ftp:",  "gopher",  "gopher:",  "file",  "file:",  "ws",  "ws:",  "wss",  "wss:" };


static const std::regex ForbiddenHostChars("/[\0\t\n\r #%/:<>?@[\\\]^|]/");
static const std::regex ForbiddenHostCharsIpv6("/[\0\t\n\r #%/<>?@\\^|]/");

FURL::FURL(const std::string& Link)
{

}

void FURL::Parse(const std::string& Url, bool bSlashesDenoteHost, bool bParseQueryString)
{
	// https://github.com/nodejs/node/blob/main/lib/url.js#L227
	// https://nodejs.org/api/url.html to see how a URL is built
	if (Url.length() == 0) return;
	bool bHasHash = false, bHasAt = false; // literally # @
	int Start = -1;
	int End = -1;
	std::string Rest;
	int LastPos = 0;

	bool bInWs = false, bSplit = false;
	for (int i = 0; i < Url.length(); ++i)
	{
		const char C = Url[i];
		// Find first and last non-whitespace characters for trimming
		const int bIsWs = isspace(C);
		if (Start == -1)
		{
			if (bIsWs) continue;
			LastPos = Start = i;
		}
		else if (bInWs)
		{
			if (!bIsWs)
			{
				End = -1;
				bInWs = false;
			}
		}
		else if (bIsWs)
		{
			End = i;
			bInWs = true;
		}

		// Only convert backslashes while we haven't seen a split character
		if (!bSplit)
		{
			switch (C)
			{
				case '@':
					bHasAt = true;
					break;
				case '#':
					bHasHash = true;
					// Fall through
				case '?':
					bSplit = true;
					break;
				case '\\':
					if (i - LastPos > 0) Rest += Url.substr(LastPos, i - LastPos);
					Rest += "/";
					LastPos = i + 1;
					break;
				default:break;
			}

		}
		else if (!bHasHash && C == '#')
		{
			bHasHash = true;
		}
	}

	// Check if string was non-empty (including strings with only whitespace)

	if (Start != -1)
	{
		if (LastPos == Start)
		{
			// We didn't convert any backslashes
			if (End == -1)
			{
				if (Start == 0)
				{
					Rest = Url;
				}
				else
				{
					Rest = Url.substr(Start, Url.length() - Start);
				}
			}
			else
			{
				Rest = Url.substr(Start, End);
			}
		}
		else if (End == -1 && LastPos < Url.length())
		{
			// We converted some backslashes and have only part of the entire string
			Rest += Url.substr(LastPos, Url.length() - LastPos);
		}
		else if (End != -1 && LastPos < End)
		{
			// We converted some backslashes and have only part of the entire string
			Rest += Url.substr(LastPos, End);
		}
	}

	if (!bSlashesDenoteHost && !bHasHash && !bHasAt)
	{
		std::smatch SimplePath = Exec(Url, SimplePathPattern);

		if (SimplePath.length() > 0)
		{
			Path = Rest;
			if (SimplePath.length() > 1)
			{
				std::string Search = SimplePath[2];
				if (bParseQueryString)
				{
					//Query = Search.substr(1, Search.length() - 1);
				}
				else
				{
					Query = Search.substr(1, Search.length() - 1);
				}
			}
			else if (bParseQueryString)
			{

			}
			return;
		}
	}

	std::smatch Proto = Exec(Rest, ProtocolPattern);

	if (Proto.length() > 0)
	{
		Scheme = Proto[0];
		ToLower(Scheme);
		Rest = Rest.substr(Scheme.length(), Rest.length() - Scheme.length());
	}

	// Figure out if it's got a host
	// user@server is *always* interpreted as a hostname, and url
	// resolution will treat //foo/bar as host=foo,path=bar because that's
	// how the browser resolves relative URLs.

	// https://stackoverflow.com/questions/6277646/in-c-check-if-stdvectorstring-contains-a-certain-value
	bool bIsHostlessProtocol = HostlessProtocol.find(Scheme) != HostlessProtocol.end();

	bool bHasSlashes = false;
	if (bSlashesDenoteHost || Proto.length() > 0 || Test(Rest, HostPattern))
	{
		bHasSlashes = Rest[0] == '/' && Rest[1] == '/';
		if (bHasSlashes && !(Proto.length() > 0 && bIsHostlessProtocol))
		{
			Rest = Rest.substr(2, Rest.length() - 2);
			bHasSlashes = true;
		}
	}

	bool bIsSlashedProtocol = SlashedProtocol.find(Proto[0]) != SlashedProtocol.end();

	if (!bIsHostlessProtocol && (bHasSlashes || (Proto.length > 0 && !bIsSlashedProtocol)))
	{
		int HostEnd = -1, AtSign = -1, NonHost = -1;
		for (int i = 0; i < Rest.length(); ++i)
		{
			switch (Rest[i])
			{
				case '	': // tab
				case '\n':
				case '\r':
					// WHATWG URL removes tabs, newlines, and carriage returns. Let's do that too.
					Rest = Rest.substr(0, i) + Rest.substr(i, Rest.length() - i);
					i -= 1;
					break;
				case ' ': // Space
				case '"':
				case '%':
				case '\'':
				case ';':
				case '<':
				case '>':
				case '\\':
				case '^': // Circumflex Accent
				case '`': // Grave Accent
				case '{':
				case '}':
				case '|':
					// Characters that are never ever allowed in a hostname from RFC 2396
					if (NonHost == -1)
					{
						NonHost = i;
					}
					break;
				case '#':
				case '/':
				case '?':
					// Find the first instance of any host-ending characters
					if (NonHost == -1)
					{
						NonHost = i;
					}
					HostEnd = i;
					break;
				case '@':
					// At this point, either we have an explicit point where the
					// auth portion cannot go past, or the last @ char is the decider.
					AtSign = i;
					NonHost = -1;
					break;
			}
			if (HostEnd != -1)
			{
				break;
			}
		}
		Start = 0;
		if (AtSign != -1)
		{
			User = Rest.substr(0, AtSign + 1); // DECODE URI COMPONENT to put into password also
			Start = AtSign + 1;
		}
		if (NonHost == -1)
		{
			Host = Rest.substr(Start, Rest.length() - Start);
			Rest = "";
		}
		else
		{
			Host = Rest.substr(Start, NonHost - Start);
			Rest = Rest.substr(NonHost, Rest.length() - NonHost);

		}

		// Parse Host
		std::smatch PortResult = Exec(Host, PortPattern);
		if (PortResult.length() > 0)
		{
			std::string Result = PortResult[0];
			if (Result[0] != ':')
			{
				Port = Result.substr(1, Result.length() - 1);
			}
			Host = Host.substr(0, Host.length() - Port.length());
		}

		const bool bIsIpv6HostName = IsIpv6HostName(Host);
		if (!bIsIpv6HostName)
		{
			Rest = GetHostName(Rest, Host, Url);
		}

	}
}

std::string FURL::GetHostName(const std::string& Rest, const std::string HostName&, const std::string& Url)
{
	for (int i = 0; i < HostName.length(); ++i)
	{
		char C = HostName[i];
		const bool bIsValid = C != '/'
			&& C != '\\'
			&& C != '#'
			&& C != '?'
			&& C != ',';
		if (!bIsValid)
		{
			Host = HostName.substr(0, i + 1);
			return 
		}
	}
}

bool FURL::IsIpv6HostName(const std::string& HostName)
{
	return HostName.length() > 1 && HostName[0] == '[' && HostName[HostName.length() - 1] == ']';
}

std::smatch FURL::Exec(const std::string& String, const std::regex& Regex)
{
	// https://stackoverflow.com/questions/44540711/what-is-returned-in-stdsmatch-and-how-are-you-supposed-to-use-it
	std::smatch Result;
	std::string::const_iterator Start(String.begin());
	while (regex_search(Start, String.cend(), Result, Regex))
	{
		Start = Result.suffix().first;
	}
	return Result;
}

bool FURL::Test(const std::string& String, const std::regex& Regex)
{
	return Exec(String, Regex).length() > 1;
}

void FURL::ToLower(const std::string& String)
{
	std::transform(String.begin(), String.end(), String.begin(), [](unsigned char c)
		{
			return std::tolower(c);
		});
}