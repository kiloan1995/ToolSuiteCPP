#include <string>
#include "../Public/URL.h"
#include <iostream>
#include <regex>
#include <set>
#include "../../Insights/Public/Logger.h"

/*
NodeJS regex

+ means 1..* token
? means optional
! means NOT
* means 0..* token
. means anything preceeding the token except a new line. amount of dots mark the amount of token to take into account
\ means that it cancels everything out that follows it. \. means that only a dot is searched.
\w anything thats a word character
\W anything thats NOT a word character
\d means digits
\s any form of whitespace
\S anything thats NOT a whitespace
{} if it contains 2 numbers its min and max number of chars to match. if it contains 1 number its the exact number of chars to match.
[] means any words that start with any of the chars inside of the brackets.
a-z means any of the characters between a and z
^ means match beginning of the line
$ match the end of the statement
() anything inside these bracket is a group. For separate comparisons
/i means case insensitive
/g is global (IGNORE THIS SHIT)
/m means multiline flag. influences ^ $
*/

// Node.js source code


static const int HostNameMaxLength = 255;

static const std::set<std::string> HostlessProtocol{ "javascript", "javascript:" };
static const std::set<std::string> SlashedProtocol{ "http", "http:",  "https",  "https:",  "ftp",  "ftp:",  "gopher",  "gopher:",  "file",  "file:",  "ws",  "ws:",  "wss",  "wss:" };
static const std::set<std::string> UnsafeProtocol{ "javascript", "javascript:" };

// Escaped characters. Use empty strings to fill up unused entries.
static const std::vector<std::string> EscapedCodes{
	/* 0 - 9 */ "", "", "", "", "", "", "", "", "", "%09",
	/* 10 - 19 */ "%0A", "", "", "%0D", "", "", "", "", "", "",
	/* 20 - 29 */ "", "", "", "", "", "", "", "", "", "",
	/* 30 - 39 */ "", "", "%20", "", "%22", "", "", "", "", "%27",
	/* 40 - 49 */ "", "", "", "", "", "", "", "", "", "",
	/* 50 - 59 */ "", "", "", "", "", "", "", "", "", "",
	/* 60 - 69 */ "%3C", "", "%3E", "", "", "", "", "", "", "",
	/* 70 - 79 */ "", "", "", "", "", "", "", "", "", "",
	/* 80 - 89 */ "", "", "", "", "", "", "", "", "", "",
	/* 90 - 99 */ "", "", "%5C", "", "%5E", "", "%60", "", "", "",
	/* 100 - 109 */ "", "", "", "", "", "", "", "", "", "",
	/* 110 - 119 */ "", "", "", "", "", "", "", "", "", "",
	/* 120 - 125 */ "", "", "", "%7B", "%7C", "%7D"
};

FURL::FURL(const std::string& Link)
{
	Init();
	Parse(Link, false, false);
	ULogger::Log("Scheme", Scheme, "User", User, "Password", Password, "Host", Host, "Port", Port, "Path", Path, "Query", Query, "Fragment", Fragment);
}

void FURL::Init()
{
	// Reference: RFC 3986, RFC 1808, RFC 2396
	// f‰ngt mit // an 
	// mindestens einmal weder / noch @
	// dann kommt irgendwann ein @
	// mindestens einmal weder / noch @
	HostPattern = std::regex(R"(^\/\/[^@/]+@[^@/]+)");

	// ende mit : und
	// 0-9 ist enthalten, 0...*
	PortPattern = std::regex(":[0-9]*$");

	//am anfang des strings muss ein buchstabe, zahl oder.+- sein. danach ein doppelpunkt
	//ohne case
	ProtocolPattern = std::regex("^[a-z0-9.+-]+:", std::regex_constants::icase);

	// Special case for a simple path URL
	// ^$ heiﬂt der ganze string
	// ein / notwendig, zweiter / ist optional aber nur wenn danach kein dritter kommt
	// kein fragezeichen oder whitespaces
	// ein fragezeichen
	// keine whitespaces, aber beliebig oft:  optional
	// 
	SimplePathPattern = std::regex(R"(^(\/\/?(?!/)[^?\\s]*)(\?[^\\s]*)?$)");
	ForbiddenHostChars = std::regex(R"([\0\t\n\r #%/:<>?@[\\\]^|])");
	ForbiddenHostCharsIpv6 = std::regex(R"([\0\t\n\r #%/<>?@\\^|])");
}

void FURL::Parse(const std::string& Url, bool bSlashesDenoteHost, bool bParseQueryString)
{
	// https://github.com/nodejs/node/blob/main/lib/url.js#L227
	// https://nodejs.org/api/url.html to see how a URL is built
	if (Url.length() == 0) return;
	bool bHasHash = false, bHasAt = false; // literally # @
	int Start = -1;
	int End = -1;
	std::string Rest = "";
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
				[[fallthrough]];
			case '?':
				bSplit = true;
				break;
			case '\\':
				if (i - LastPos > 0)
				{
					Rest += Url.substr(LastPos, (i + 1) - LastPos);
				}
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
				Rest = Url.substr(Start, (End + 1) - Start);
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
			Rest += Url.substr(LastPos, (End + 1) - LastPos);
		}
	}

	if (!bSlashesDenoteHost && !bHasHash && !bHasAt)
	{
		std::vector<std::string> SimplePath = Exec(Rest, SimplePathPattern);

		if (SimplePath.size() > 0)
		{
			Path = SimplePath[1];
			if (SimplePath.size() > 1)
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
			ULogger::Log("Is Simple path: return early");
			return;
		}
	}

	std::vector<std::string> Proto = Exec(Rest, ProtocolPattern);

	if (Proto.size() > 0)
	{
		Scheme = Proto[0];
		ToLower(Scheme);
		Rest = Rest.substr(Scheme.length(), Rest.length() - (Scheme.length() - 1));
	}

	// Figure out if it's got a host
	// user@server is *always* interpreted as a hostname, and url
	// resolution will treat //foo/bar as host=foo,path=bar because that's
	// how the browser resolves relative URLs.

	// https://stackoverflow.com/questions/6277646/in-c-check-if-stdvectorstring-contains-a-certain-value
	bool bIsHostlessProtocol = HostlessProtocol.find(Scheme) != HostlessProtocol.end();

	bool bHasSlashes = false;
	if (bSlashesDenoteHost || Proto.size() > 0 || Test(Rest, HostPattern))
	{
		bHasSlashes = Rest[0] == '/' && Rest[1] == '/';
		if (bHasSlashes && !(Proto.size() > 0 && bIsHostlessProtocol))
		{
			Rest = Rest.substr(2, Rest.length() - 2);
			bHasSlashes = true;
		}
	}

	bool bIsSlashedProtocol = SlashedProtocol.find(Scheme) != SlashedProtocol.end();

	if (!bIsHostlessProtocol && (bHasSlashes || (Proto.size() > 0 && !bIsSlashedProtocol)))
	{
		// there's a hostname.
		// the first instance of /, ?, ;, or # ends the host.
		//
		// If there is an @ in the hostname, then non-host chars *are* allowed
		// to the left of the last @ sign, unless some host-ending character
		// comes *before* the @-sign.
		// URLs are obnoxious.
		//
		// ex:
		// http://a@b@c/ => user:a@b host:c
		// http://a@b?@c => user:a host:b path:/?@c

		int HostEnd = -1, AtSign = -1, NonHost = -1;
		for (int i = 0; i < Rest.length(); ++i)
		{
			switch (Rest[i])
			{
			case '\t': // tab
			case '\n':
			case '\r':
				// WHATWG URL removes tabs, newlines, and carriage returns. Let's do that too.
				Rest = Rest.erase(i, 1);
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
		std::vector<std::string> PortResult = Exec(Host, PortPattern);
		if (PortResult.size() > 0)
		{
			std::string Result = PortResult[0];
			if (Result != ":")
			{
				Port = Result.substr(1, Result.length() - 1);
			}
			Host = Host.substr(0, Host.length() - Port.length());
		}


		const bool bIsIpv6HostName = IsIpv6HostName(Host);
		if (!bIsIpv6HostName)
		{
			Rest = GetHostName(Rest, Host); // Maybe rename to SPLITHOSTNAME
		}

		if (Host.length() > HostNameMaxLength)
		{
			Host = "";
		}
		else
		{
			// Hostnames are always lower case.
			ToLower(Host);
		}


		if (Host.length() > 0)
		{
			if (bIsIpv6HostName)
			{
				if (Test(Host, ForbiddenHostCharsIpv6))
				{
					ULogger::Log("Error, URL contains forbidden Ipv6 HostChars", Url);
				}
			}
			else
			{
				// NODEJS does ASCII CONVERSION HERE
				if (Test(Host, ForbiddenHostChars))
				{
					ULogger::Log("Error, URL contains forbidden HostChars", Url);
				}
			}
		}
		// BLah blah nodejs, we already have the host and part separated, no need to again add port to host.
		//Host += Port.length() > 0 ? ":" + Port : "";

		// strip [ and ] from the hostname
		// the host field still retains them, though
		if (bIsIpv6HostName)
		{
			Host = Host.substr(1, Host.length() - 2); // Removes first and last char
			if (Rest[0] != '/')
			{
				Rest.insert(0, "/");
			}
		}
	}
	// Now rest is set to the post-host stuff.
	// Chop off any delim chars
	bool bIsUnsafeProtocol = UnsafeProtocol.find(Scheme) != UnsafeProtocol.end();

	if (!bIsUnsafeProtocol)
	{
		// First, make 100% sure that any "autoEscape" chars get
		// escaped, even if encodeURIComponent doesn't think they
		// need to be.
		Rest = AutoEscapeAllDelimiters(Rest);
	}
	int QuestionIdx = -1;
	int HashIdx = -1;

	for (int i = 0; i < Rest.length(); ++i)
	{
		const char C = Rest[i];
		if (C == '#')
		{
			Fragment = Rest.substr(i, Rest.length() - i);
			HashIdx = i;
			break;
		}
		else if (C == '?' && QuestionIdx == -1)
		{
			QuestionIdx = i;
		}
	}

	if (QuestionIdx != -1)
	{
		if (HashIdx == -1)
		{
			Query = Rest.substr(QuestionIdx, Rest.length() - QuestionIdx);
		}
		else
		{
			Query = Rest.substr(QuestionIdx, HashIdx - (QuestionIdx - 1));
		}
		if (bParseQueryString)
		{
			// NODEJS DOES SOME PARSING HERE
		}
	}
	else if (bParseQueryString)
	{
		// No query string, but parseQueryString still requested
		//NODEJS DOES SOME STUPID STUFF HERE AGAIN
	}
	const bool bUseQuestionIdx = QuestionIdx != -1 && (HashIdx == -1 || QuestionIdx < HashIdx);
	int FirstIdx = bUseQuestionIdx ? QuestionIdx : HashIdx;
	if (FirstIdx == -1)
	{
		if (Rest.length() > 0)
		{
			Path = Rest;
		}
	}
	else if (FirstIdx > 0)
	{
		Path = Rest.substr(0, FirstIdx);
	}
	if (bIsSlashedProtocol && Host.length() > 0 && Path.length() == 0)
	{
		Path = "/";
	}
}

std::string FURL::GetHref()
{
	std::string Href = Scheme;
	if (Scheme.length() > 0 && Scheme[Scheme.length() - 1] != ':')
	{
		Href = ":";
	}

	// Only the slashedProtocols get the //.  Not mailto:, xmpp:, etc.
	// unless they had them to begin with.
	bool bIsSlashedProtocol = SlashedProtocol.find(Scheme) != SlashedProtocol.end();
	if (bIsSlashedProtocol && Host.length() > 0)
	{
		if (Path.length() > 0 && Path[0] != '/')
		{
			ULogger::Log("slahred");
			Href += "/";
		}
		else if (Scheme.length() >= 4
			&& Scheme[0] == 'f'
			&& Scheme[1] == 'i'
			&& Scheme[2] == 'l'
			&& Scheme[3] == 'e'
			)
		{
			Href += "//";
		}
	}

	if (User.length() > 0)
	{
		Href = User + ":" + Password + "@";
	}
	if (Host.length() > 0)
	{
		Href += Host;
	}
	if (Port.length() > 0)
	{
		Href += ":" + Port;
	}
	if (Path.length() > 0)
	{
		Href += Path;
	}
	if (Query.length() > 0)
	{
		Href += Query;
	}
	return Href;
}

std::string FURL::AutoEscapeAllDelimiters(const std::string& String)
{
	std::string Escaped;
	int LastEscapedPos = 0;
	for (int i = 0; i < String.length(); ++i)
	{
		// `escaped` contains substring up to the last escaped character.
		std::string EscapedChar = EscapedCodes[static_cast<uint8_t>(String[i])];
		if (EscapedChar.length() > 0)
		{
			// Concat if there are ordinary characters in the middle.
			if (i > LastEscapedPos)
			{
				Escaped += String.substr(LastEscapedPos, i - LastEscapedPos);
			}
			Escaped += EscapedChar;
			LastEscapedPos = i + 1;
		}
	}
	if (LastEscapedPos == 0) return String; // Nothing has been escaped.

	if (LastEscapedPos < String.length())
	{
		Escaped += String.substr(LastEscapedPos, String.length() - LastEscapedPos);
	}
	return Escaped;
}

std::string FURL::GetHostName(const std::string& Rest, const std::string& HostName)
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
			return HostName.substr(i + 1, HostName.length() - (i + 1)) + Rest;
		}
	}
	return Rest;
}

bool FURL::IsIpv6HostName(const std::string& HostName)
{
	return HostName.length() > 1 && HostName[0] == '[' && HostName[HostName.length() - 1] == ']';
}

std::vector<std::string> FURL::Exec(const std::string& String, std::regex Regex)
{
	std::vector<std::string> Matches;
	// https://stackoverflow.com/questions/44540711/what-is-returned-in-stdsmatch-and-how-are-you-supposed-to-use-it
	std::smatch Result;
	std::string::const_iterator Start(String.begin());
	while (regex_search(Start, String.cend(), Result, Regex))
	{
		Matches.push_back(Result[0]);
		Start = Result.suffix().first;
	}
	return Matches;
}

bool FURL::Test(const std::string& String, std::regex Regex)
{
	std::smatch Result;
	std::string::const_iterator Start(String.begin());
	while (regex_search(Start, String.cend(), Result, Regex))
	{
		return true;
	}
	return false;
}

void FURL::ToLower(std::string& String)
{
	std::transform(String.begin(), String.end(), String.begin(), [](unsigned char c)
		{
			return std::tolower(c);
		});
}