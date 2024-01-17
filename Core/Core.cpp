#include "Core.h"
#include "HTTP/Public/Socket.h"
#include "Insights/Public/Logger.h"
#include "HTTP/Public/URL.h"

UCore::UCore()
{
	ULogger::Log("Hello World");

	//std::string Url = "https://c0001160288t1.valhalla12.stage.jobs2web.com/";
	std::string Url = "https://github.com:44255/nodejs/node/blob/main/lib/url.js?jobId=12&template=900#L441";
	FURL URL = FURL(Url);
	ULogger::Log(URL.GetHref());
	//USocket Socket = USocket();
	//Socket.TestClient();
}
