#pragma once

BOOL NodeExistsInParent(CComPtr<ItcProjectNode> owner_node, CComBSTR name);
CComBSTR GetCaptionFromFileName(const TCHAR* file_name);
HRESULT ForceDirectoriesForFile(const TCHAR* file_name);

extern const wchar_t __declspec(selectany) cDefaultName[]          = L"VersionOne";
extern const wchar_t __declspec(selectany) cDefaultFileExtension[] = L"tcV1";
extern const wchar_t __declspec(selectany) cDefaultFolder[]        = L"VersionOne";
extern const wchar_t __declspec(selectany) cFileNameProperty[]     = L"FileName";
