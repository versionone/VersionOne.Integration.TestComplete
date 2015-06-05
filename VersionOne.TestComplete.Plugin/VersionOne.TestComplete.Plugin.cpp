// VersionOne.TestComplete.Plugin.cpp : Implementation of DLL Exports.
#include "stdafx.h"
#include "resource.h"
#include "VersionOneTestCompletePlugin_i.h"
#include "dllmain.h"
#include "Common\aqExtensionRegistrator.h"
#include "Plugin.h"

EXTERN_C HRESULT STDAPICALLTYPE GetPluginRegistrator(IaqExtensionRegistrator** lpRetVal)
{
	CComObject<CaqExtensionRegistrator>* pRegistrator;
	HRESULT hr = CComObject<CaqExtensionRegistrator>::CreateInstance(&pRegistrator);
	if (FAILED(hr)) return hr;

    // {44280609-C22C-4fae-A92A-7C6735278802}
    CONST GUID key = {0x44280609, 0xc22c, 0x4fae, {0xa9, 0x2a, 0x7c, 0x67, 0x35, 0x27, 0x88, 0x2}};

    pRegistrator->SetDescription(key, cRegistratorName, cCopyright);

	AQ_PLUGIN_VERSION version = {1, 0};
    pRegistrator->AddPlugin(CPlugin::CreatePlugin,
	              CPlugin::GetPluginKey(),
	              cPluginName,
	              cPluginDescription,
	              version);

	return pRegistrator->QueryInterface(lpRetVal);
}
