// SamplePlugInConfiguration.cpp : Implementation of CPluginConfiguration

#include "stdafx.h"
#include "PluginConfiguration.h"

// CPluginConfiguration

CPluginConfiguration::CPluginConfiguration()
{
}


void CPluginConfiguration::Initialize(CComBSTR name,
                                            CComBSTR location,
                                            CComPtr<ItcProjectNode> owner,
                                            CComPtr<IaqMessenger>   messenger)
{
	m_name         = name;
	m_location     = location;
	m_owner        = owner;
	m_messenger    = messenger;
}


// ItcProjectNodeConfigurationInfo Methods

HRESULT STDMETHODCALLTYPE CPluginConfiguration::get_ParameterCount(long * Value)
{
	_ASSERT( Value != NULL );
	*Value = 2;
	return S_OK;
}


HRESULT STDMETHODCALLTYPE CPluginConfiguration::get_ParameterCaption(long Index, BSTR * Value)
{
	_ASSERT( Value != NULL );
	switch (Index)
	{
		case 0:  return CComBSTR(_T("Name")    ).CopyTo(Value);
		case 1:  return CComBSTR(_T("Location")).CopyTo(Value);
		default: return ERROR_BAD_ARGUMENTS;
	}
}


HRESULT STDMETHODCALLTYPE CPluginConfiguration::get_ParameterValue(long Index, VARIANT * Value)
{
	_ASSERT( Value != NULL );
	switch (Index)
	{
		case 0:  return m_name    .CopyTo(Value);
		case 1:  return m_location.CopyTo(Value);
		default: return ERROR_BAD_ARGUMENTS;
	}
}


HRESULT STDMETHODCALLTYPE CPluginConfiguration::put_ParameterValue(long Index, VARIANT Value)
{
	switch (Index)
	{
		case 0:  return CComVariant(Value).CopyTo(&m_name);
		case 1:  return CComVariant(Value).CopyTo(&m_location);
		default: return ERROR_BAD_ARGUMENTS;
	}
}


HRESULT STDMETHODCALLTYPE CPluginConfiguration::IsValid(long * InvalidParamIndex, VARIANT_BOOL * Value)
{
	_ASSERT( Value != NULL );
	_ASSERT( InvalidParamIndex != NULL );

	*Value = VARIANT_FALSE;

	HRESULT hr;

	// Checks whether parent has a child with the same name
	if (NodeExistsInParent(m_owner, m_name))
	{
		*InvalidParamIndex = 0;
		CComBSTR msg(_T("The parent node already contain a child with the specified name."));
		hr = m_messenger->ShowMildError(msg, 0, CComBSTR(_T("")));
		if (FAILED(hr)) return hr;
		return S_OK;
	}

	// Checks whether the specified file already exists
	_bstr_t path = _bstr_t(m_location) + _bstr_t(m_name) + "." + cDefaultFileExtension;
	if (GetFileAttributes(path) != 0xffffffff)
	{
		CComBSTR msg(_T("The specified file already exists. Would you like to overwrite it?"));
		VARIANT_BOOL res;
		hr = m_messenger->YesNoConfirm(msg, 0, CComBSTR(""), VARIANT_FALSE, &res);
		if (FAILED(hr)) return hr;
		if (res == VARIANT_FALSE)
		{
			*InvalidParamIndex = 1;
			return S_OK;
		}
	}

	*Value = VARIANT_TRUE;
	return S_OK;
}
