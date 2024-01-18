#include "Core.h"
#include "HAL/Public/PlatformMisc.h"
#include "Insights/Public/Logger.h"
#include "Networking/HTTP/Public/HTTPRequest.h"
#include "Networking/Online/Public/URL.h"

UCore::UCore()
{
	ULogger::Log("ToolSuiteCPP Started. Welcome");
	OnlineService.StartUp();

	//std::string Url = "https://c0001160288t1.valhalla12.stage.jobs2web.com/";
	//std::string Url = "https://github.com:44255/nodejs/node/blob/main/lib/url.js?jobId=12&template=900#L441";

	std::string Href = "http://127.0.0.1:5001/dashboardui-rs/europe-west1/getAllApplicationsAtJob";
	FURL URL = FURL(Href);

	UHTTPRequest Request;
	Request.Process(&URL);

	//ULogger::Log(URL.GetHref());
	//USocket Socket = USocket();
	//Socket.TestClient();

	OnlineService.Shutdown();
}
