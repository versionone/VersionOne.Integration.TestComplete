// SamplePlugInConfiguration.h : Declaration of the CPluginConfiguration

#pragma once

// CPluginConfiguration

class ATL_NO_VTABLE CPluginConfiguration : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPluginConfiguration>,
	public ItcProjectNodeConfigurationInfo
{
	BEGIN_COM_MAP(CPluginConfiguration)
		COM_INTERFACE_ENTRY(ItcProjectNodeConfigurationInfo)
	END_COM_MAP()

public:
	CPluginConfiguration();
	void Initialize(CComBSTR                name,
	                CComBSTR                location,
	                CComPtr<ItcProjectNode> owner,
	                CComPtr<IaqMessenger>   messenger);

	// ItcProjectNodeConfigurationInfo Methods
	STDMETHOD(get_ParameterCount)(long * Value);
	STDMETHOD(get_ParameterCaption)(long Index, BSTR * Value);
	STDMETHOD(get_ParameterValue)(long Index, VARIANT * Value);
	STDMETHOD(put_ParameterValue)(long Index, VARIANT Value);
	STDMETHOD(IsValid)(long * InvalidParamIndex, VARIANT_BOOL * Value);

private:
	CComBSTR                m_name;
	CComBSTR                m_location;
	CComPtr<ItcProjectNode> m_owner;
	CComPtr<IaqMessenger>   m_messenger;
};
