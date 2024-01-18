#include "../Public/OnlineService.h"
#include "../../../Insights/Public/Logger.h"

void UOnlineService::StartUp()
{
	ULogger::Log("UOnlineService: StartUp");
	// https://learn.microsoft.com/de-de/windows/win32/api/winsock2/nf-winsock2-wsastartup
	const WORD DllVersion = MAKEWORD(2, 2);
	bInitializedSuccessfully = WSAStartup(DllVersion, &WSAData) == 0;
	if (!bInitializedSuccessfully)
	{
		ULogger::Log("Error: WSAStartup failed");
		WSACleanup();
	}
	else
	{
		ULogger::Log("Success: WSAStartup");
	}
}

void UOnlineService::Shutdown()
{
	if (bInitializedSuccessfully)
	{
		WSACleanup();
		ULogger::Log("UOnlineService: Shutdown");
	}
}