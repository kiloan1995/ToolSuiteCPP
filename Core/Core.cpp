#include "Core.h"
#include <iostream>
#include <string>
#include "Math/Public/Math.h"
#include "Math/Public/Vector3.h"
#include "HTTP/Public/URL.h"

UCore::UCore()
{
	std::cout << "Hello World in Core\n";
	FURL URL = FURL("https://c0001160288t1.valhalla12.stage.jobs2web.com/");
}