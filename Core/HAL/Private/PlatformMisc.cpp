#include "../Public/PlatformMisc.h"
#include <windows.h>
#include <iostream>

bool FPlatformMisc::ClipBoardCopy(const std::string& String)
{
	// https://cplusplus.com/forum/general/48837/
	// https://learn.microsoft.com/de-de/windows/win32/dataxchg/using-the-clipboard?redirectedfrom=MSDN#_win32_Example_of_a_Clipboard_Viewer

	if (String.length() > 0)
	{
		// Open the clipboard, and empty it.
		if (!OpenClipboard(nullptr))
		{
			return false;
		}
		EmptyClipboard();

		// Allocate a global memory object for the text (in bytes, not number of chars). 
		if (const HGLOBAL HMem = GlobalAlloc(GMEM_MOVEABLE, String.size() + 1))
		{
			// Lock the handle and copy the text to the buffer. 
			memcpy(GlobalLock(HMem), String.c_str(), String.size() + 1);
			GlobalUnlock(HMem);
			// Place the handle on the clipboard.
			SetClipboardData(CF_UNICODETEXT, HMem);
			CloseClipboard();
			GlobalFree(HMem);
		}
		else // Close in any case
		{
			CloseClipboard();
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool FPlatformMisc::ClipBoardPaste(std::string& String)
{
	// https://stackoverflow.com/questions/14762456/getclipboarddatacf-text

	if (!IsClipboardFormatAvailable(CF_TEXT)) return false;
	if (!OpenClipboard(nullptr)) return false;
	if (const HGLOBAL HMem = GetClipboardData(CF_TEXT))
	{
		if (const char* Chars = static_cast<char*>(GlobalLock(HMem)))
		{
			String = std::string(Chars);
		}
		GlobalUnlock(HMem);
		CloseClipboard();
	}
	else
	{
		CloseClipboard();
		return false;
	}
	return true;

}