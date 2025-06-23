#pragma once
#include "pch.h"
#include "ErrorHandling.h"

void ErrorHandler::DisplayError() {
	//Code to convert GetLastError() string to something usable
	LPWSTR messageBuffer = nullptr;
	//Parameters are flags to tell FormatMessage how to format, and what to format.
	size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);
	//Put the buffer into a wide string
	std::wstring message(messageBuffer, size);
	free(messageBuffer);
	OutputDebugString(L"Error Occured");
	OutputDebugString(message.c_str());
}
