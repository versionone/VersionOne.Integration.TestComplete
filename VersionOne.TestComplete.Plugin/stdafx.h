// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <io.h>
#include <vector>
#include "Common\aqConsts.h"
#include "Common\tcProductKeys.h"
#include "Common\ActionsConst.h"
#include "Common\tcConst.h"
#include "Common\tcProjectNodeActionKey.h"
#include "Const.h"

#import "C:\tools\AutomatedQA\TestComplete 8\Bin\TestComplete.exe" \
	raw_interfaces_only, raw_native_types, named_guids, auto_search, no_namespace, \
	exclude("tagRECT", "UINT_PTR"), \
	rename("PlaySound", "PlaySoundR"), \
	rename("GetMessage", "GetMessageR")

#import "C:\tools\AutomatedQA\TestComplete 8\Bin\TestCompleteService8.exe" \
	raw_interfaces_only, raw_native_types, named_guids, auto_search, no_namespace, \
	exclude("PUINT_PTR", "tagPOINT", "IEnumUnknown", "_FILETIME", "AQ_PROPERTY_TYPE", "ITypeLib", "ITypeInfo"), \
	exclude("tagTYPEATTR", "tagTYPEKIND", "tagTYPEDESC", "__MIDL_IOleAutomationTypes_0005", "tagARRAYDESC"), \
	exclude("tagSAFEARRAYBOUND", "tagIDLDESC", "DWORD", "ITypeComp", "tagDESCKIND", "tagFUNCDESC", "tagELEMDESC"), \
	exclude("tagPARAMDESC", "tagPARAMDESCEX", "tagFUNCKIND", "tagINVOKEKIND", "tagCALLCONV", "tagVARDESC"), \
	exclude("__MIDL_IOleAutomationTypes_0006", "tagVARKIND", "tagTLIBATTR", "tagSYSKIND")

#import "C:\tools\AutomatedQA\TestComplete 8\Bin\Extensions\TestCompleteInternal.tlb" \
	raw_interfaces_only, raw_native_types, named_guids, auto_search, no_namespace, \
	exclude("PUINT_PTR", "tagPOINT", "IEnumUnknown", "_FILETIME", "AQ_PROPERTY_TYPE", "ITypeLib", "ITypeInfo"), \
	exclude("tagTYPEATTR", "tagTYPEKIND", "tagTYPEDESC", "__MIDL_IOleAutomationTypes_0005", "tagARRAYDESC"), \
	exclude("tagSAFEARRAYBOUND", "tagIDLDESC", "DWORD", "ITypeComp", "tagDESCKIND", "tagFUNCDESC", "tagELEMDESC"), \
	exclude("tagPARAMDESC", "tagPARAMDESCEX", "tagFUNCKIND", "tagINVOKEKIND", "tagCALLCONV", "tagVARDESC"), \
	exclude("__MIDL_IOleAutomationTypes_0006", "tagVARKIND", "tagTLIBATTR", "tagSYSKIND"), \
	exclude("TestCompleteServiceLib"), \
	rename("GetUserName", "GetUserNameR"), \
	rename("GetWindowsDirectory", "GetWindowsDirectoryR"), \
	rename("GetSystemDirectory", "GetSystemDirectoryR"), \
	rename("GetFileVersionInfo", "GetFileVersionInfoR"), \
	rename("CreateProcess", "CreateProcessR"), \
	rename("FindWindow", "FindWindowR"), \
	rename("FindWindowEx", "FindWindowExR"), \
	rename("GetMenuItemInfo", "GetMenuItemInfoR"), \
	rename("RegisterWindowMessage", "RegisterWindowMessageR"), \
	rename("CreateProcess", "CreateProcessR"), \
	rename("FindWindow", "FindWindowR"), \
	rename("FindWindowEx", "FindWindowExR"), \
	rename("GetMenuItemInfo", "GetMenuItemInfoR"), \
	rename("PostMessage", "PostMessageR"), \
	rename("PostThreadMessage", "PostThreadMessageR"), \
	rename("SendMessage", "SendMessageR"), \
	rename("SendMessageTimeout", "SendMessageTimeoutR"), \
	rename("RegisterWindowMessage", "RegisterWindowMessageR"), \
	rename("GetObject", "GetObjectR"), \
	rename("SendMessageTimeout", "SendMessageTimeoutR"), \
	rename("GetObject", "GetObjectR"), \
	rename("GetFileAttributes", "GetFileAttributesR"), \
	rename("CopyFile", "CopyFileR"), \
	rename("LoadImage", "LoadImageR"), \
	rename("CreateEvent", "CreateEventR"), \
	rename("CreateProcessAsUser", "CreateProcessAsUserR"), \
	rename("VkKeyScanEx", "VkKeyScanExR"), \
	rename("MapVirtualKeyEx", "MapVirtualKeyExR"), \
	rename("DeleteFile", "DeleteFileR"), \
	rename("DISPPARAMS", "tagDISPPARAMS"), \
	rename("EXCEPINFO", "tagEXCEPINFO"), \
	rename("EOF", "EOFR"), \
	rename("CreateDirectory", "CreateDirectoryR"), \
	rename("GetClassName", "GetClassNameR"), \
	rename("SetCurrentDirectory", "SetCurrentDirectoryR"), \
	rename("StrToInt", "StrToIntR"), \
	rename("SetFileAttributes", "SetFileAttributesR"), \
	rename("MoveFile", "MoveFileR"), \
	rename("GetShortPathName", "GetShortPathNameR"), \
	rename("GetEnvironmentVariable", "GetEnvironmentVariableR"), \
	rename("SetLocaleInfo", "SetLocaleInfoR"), \
	rename("GetLocaleInfo", "GetLocaleInfoR")

using namespace ATL;

#include "common.h"

extern HINSTANCE hInstance;