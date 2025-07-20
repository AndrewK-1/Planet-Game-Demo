//pch.h is a precompiled header file that will only compile if the code it includes is changed.
//Including only standard and unchanging files here will speed up the compilation process as this file will be skipped in compiling.

#pragma once

//Specifies earliest supported verison of Windows
#include<winsdkver.h>
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601 // Targeting Windows 7 or later
#endif
//Now, include the Windows versions.
#include <sdkddkver.h>

//Prevent Windows min/max from conflicting with std::min / std::max
#define NOMINMAX

//DirectX doesn't use GDI.  These macros disable GDI to reduce compile time.
// These macros are, however, necessary for <dwrite.h> to use LOGFONTW.
//#define NODRAWTEXT
//#define NOGDI
//#define NOBITMAP

//Can be included by <mcx.h>
#define NOMCX

//Can be included by <winsvc.h>
#define NOSERVICE

//WinHelp is deprecated
#define NOHELP

//Another macro to reduce compile time
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//Something to do with creating COM objects
#include <wrl/client.h>

#include <d3d11.h>
#include <d2d1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dwrite.h>

//Recommended standard files by DirectX VS template
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <exception>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <string>
#include <vector>

//Add libraries
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")


//Template for catching DirectX API errors
namespace DX
{
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			//A breakpoint can be set here to catch API errors.
			char msg[64];
            sprintf_s(msg, "HRESULT failed: 0x%08X", hr);
            throw std::runtime_error(msg);
			throw std::exception();
		}
	}
}