#include "StdAfx.h"

BOOL NodeExistsInParent(CComPtr<ItcProjectNode> owner_node, CComBSTR name) {
	_ASSERT( owner_node != NULL );

	if ((name.Length() > 0) && FAILED(name.ToLower()))
		return FALSE;
  
	CComPtr<ItcProjectNodeIterator> iterator;
    if (FAILED( owner_node->CreateChildIterator(&iterator) )) {
        return FALSE;
    }
	
    while (iterator->HasNext() == VARIANT_TRUE)  {
		CComPtr <ItcProjectNode> node;
		
        if (FAILED( iterator->Next(&node) )) {
            return FALSE;
        }
		
        CComBSTR caption;
		
        if (SUCCEEDED( node->get_Caption(&caption) )) {
            if ((caption.Length() == 0) || SUCCEEDED(caption.ToLower())) {
                if (caption == name) {
					return TRUE;
                }
            }
        }
	}

	return FALSE;
}

CComBSTR GetCaptionFromFileName(const TCHAR* file_name) {
	_ASSERT( file_name != NULL );
	
    CONST TCHAR* s = _tcsrchr(file_name, _T('\\'));
    if (s == NULL) {
        return CComBSTR(_T(""));
    }

	TCHAR* buf = _tcsdup(s + 1);
    if (buf == NULL) {
        return CComBSTR(_T(""));
    }

	TCHAR* p = _tcsrchr(buf, _T('.'));
    if (p != NULL) {
        *p = 0;
    }

	CComBSTR rv(buf);
	free(buf);

	return rv;
}

HRESULT ForceDirectoriesForFile(const TCHAR* file_name) {
	int i;
	TCHAR fn[MAX_PATH];
	_tcscpy(fn, file_name);

	std::vector< std::basic_string<TCHAR> > paths;
    
    for (i = _tcslen(fn) - 1; i >= 0; i--) {
		if (fn[i] == '\\') {
			fn[i] = 0;
			paths.push_back(fn);
		}
    }

	for (i = paths.size() - 1; i >= 0; i--)	{
		DWORD attr = GetFileAttributes(paths[i].c_str());
		
        if (attr == 0xffffffff) {
            if (CreateDirectory(paths[i].c_str(), NULL) == FALSE) {
				return ERROR_DIRECTORY;
            }
		} else {
            if (! (attr & FILE_ATTRIBUTE_DIRECTORY)) {
				return ERROR_DIRECTORY;
            }
		}
	}

	return S_OK;
}
