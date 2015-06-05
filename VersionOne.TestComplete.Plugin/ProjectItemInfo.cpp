// SampleProjectItemInfo.cpp : Implementation of CProjectItemInfo

#include "stdafx.h"
#include "ProjectItemInfo.h"
#include "PluginConfiguration.h"

// CProjectItemInfo

CProjectItemInfo::CProjectItemInfo()
{
	m_bitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_PROJECTITEM));
	_ASSERT( m_bitmap != NULL );
}


CProjectItemInfo::~CProjectItemInfo()
{
	DeleteObject(m_bitmap);
}


void CProjectItemInfo::Initialize(CComPtr<IaqMessenger> messenger)
{
	m_messenger = messenger;
}

// ItcProjectNodeInfo Methods

HRESULT STDMETHODCALLTYPE CProjectItemInfo::get_InfoProperty(TC_PROJECT_NODE_INFO_PROP PropID, VARIANT * Value)
{
	_ASSERT( Value != NULL );

	CComVariant default_node_name     (cDefaultName);
	CComVariant bitmap                ((LONG)m_bitmap);
	CComVariant name                  (cProjectItemName);
	CComVariant description           (cProjectItemDescription);
	CComVariant description_ex        (cProjectItemDescription_ex);
	CComVariant help_context          (0);
	CComVariant help_context_file_name(_T(""));
	CComVariant help_topic            (_T(""));
	CComVariant help_topic_file_name  (_T(""));
	CComVariant is_alone              (true);
	CComVariant is_required           (false);
	CComVariant can_edit_name         (false);
	CComVariant can_delete            (true);
	CComVariant recorder_create_only  (false);

	switch (PropID)
	{
		case pniDefaultNodeName:     return VariantCopy(Value, &default_node_name);
		case pniBitmapReference:     return VariantCopy(Value, &bitmap);
		case pniName:                return VariantCopy(Value, &name);
		case pniDescription:         return VariantCopy(Value, &description);
		case pniDescriptionEx:       return VariantCopy(Value, &description_ex);
		case pniHelpContext:         return VariantCopy(Value, &help_context);
		case pniHelpContextFileName: return VariantCopy(Value, &help_context_file_name);
		case pniHelpTopic:           return VariantCopy(Value, &help_topic);
		case pniHelpTopicFileName:   return VariantCopy(Value, &help_topic_file_name);
		case pniIsAlone:             return VariantCopy(Value, &is_alone);
		case pniIsRequired:          return VariantCopy(Value, &is_required);
		case pniCanEditName:         return VariantCopy(Value, &can_edit_name);
		case pniCanDelete:           return VariantCopy(Value, &can_delete);
		case pniRecorderCreateOnly:  return VariantCopy(Value, &recorder_create_only);
		default:
			_ASSERT( false );
			return ERROR_BAD_ARGUMENTS;
	}
}


HRESULT STDMETHODCALLTYPE CProjectItemInfo::get_SupportedParentType(GUID * Value)
{
	_ASSERT( Value != NULL );
	*Value = c_ProjectNodeType_Project;
	return S_OK;
}


CComBSTR CProjectItemInfo::GetPath(CComPtr<ItcProjectNode> node)
{
	CComVariant val;
	if (FAILED(node->get_NodeProperty(pnpStorageName, &val))) return CComBSTR(_T(""));
	if (val.vt != VT_BSTR) return CComBSTR(_T(""));

	TCHAR* path = _tcsdup(_bstr_t(val));
	TCHAR* s = _tcsrchr(path, '\\');
	if (s == NULL)
		path[0] = 0;
	else
		s[1] = 0;

	CComBSTR rv_path(path);
	free(path);

	rv_path.Append(cDefaultFolder);
	rv_path.Append(_T("\\"));

	return rv_path;
}


CComBSTR CProjectItemInfo::GetName(CComPtr<ItcProjectNode> node, int index)
{
	CComBSTR name;
	do
	{
		index++;

		if (FAILED(get_DefaultFileName(&name))) return CComBSTR(_T(""));

		TCHAR number[64];
		name.Append(_itot(index, number, 10));
	}
	while (NodeExistsInParent(node, name));
	return name;
}


HRESULT STDMETHODCALLTYPE CProjectItemInfo::CreateConfigurationParams(ItcProjectNode * AParentNode, ItcProjectNodeConfigurationInfo * * Value)
{
	HRESULT hr;

	// Get location path
	CComBSTR path = GetPath(AParentNode);

	// Choose the name
	CComBSTR name = GetName(AParentNode, 0);

	CComObject<CPluginConfiguration>* conf;
	hr = CComObject<CPluginConfiguration>::CreateInstance(&conf);
	if (FAILED(hr)) return hr;
  
	conf->Initialize( name,
	                  path,
	                  AParentNode,
	                  m_messenger );

	return conf->QueryInterface(Value);
}

// ItcFileProjectNodeInfo Methods

HRESULT STDMETHODCALLTYPE CProjectItemInfo::get_DefaultFileName(BSTR * Value)
{
	_ASSERT( Value != NULL );
	CComBSTR def_name(cDefaultName);
	return def_name.CopyTo(Value);
}


HRESULT STDMETHODCALLTYPE CProjectItemInfo::get_FileExtension(BSTR * Value)
{
	_ASSERT( Value != NULL );
	CComBSTR def_ext(cDefaultFileExtension);
	return def_ext.CopyTo(Value);
}

// IaqKeyItem Methods

void STDMETHODCALLTYPE CProjectItemInfo::GetKey(GUID * Value)
{
	_ASSERT( Value != NULL );
	*Value = GetKey();
}


CONST GUID CProjectItemInfo::GetKey()
{
	// {CBE174D7-71E2-4cc3-8DCB-BD06AC85399B}
    CONST GUID key = { 0xcbe174d7, 0x71e2, 0x4cc3, { 0x8d, 0xcb, 0xbd, 0x6, 0xac, 0x85, 0x39, 0x9b } };

	return key;
}

