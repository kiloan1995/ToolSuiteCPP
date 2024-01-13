#include "Core.h"
#include "HTTP/Public/Socket.h"
#include "Insights/Public/Logger.h"

UCore::UCore()
{
	ULogger::Log("Hello World");

	//FURL URL = FURL("https://c0001160288t1.valhalla12.stage.jobs2web.com/");
	USocket Socket = USocket();
	Socket.TestClient();
}
