// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "VersionOneTestCompletePlugin_i.h"
#include "dllmain.h"

CVersionOneTestCompletePluginModule _AtlModule;
HINSTANCE hInstance;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	::hInstance = hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
