// ChildNodeIterator.h : Declaration of the CChildNodeIterator

#pragma once

// CChildNodeIterator

class ATL_NO_VTABLE CChildNodeIterator : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CChildNodeIterator>,
	public ItcProjectNodeIterator
{
	BEGIN_COM_MAP(CChildNodeIterator)
		COM_INTERFACE_ENTRY(IaqIterator)
		COM_INTERFACE_ENTRY(ItcProjectNodeIterator)
	END_COM_MAP()

public:
	// ItcProjectNodeIterator Methods
	STDMETHOD(Next)(ItcProjectNode * * Value);

	// IaqIterator Methods
	STDMETHOD_(void, Skip)(long Count);
	STDMETHOD_(VARIANT_BOOL, HasNext)();
	STDMETHOD_(void, Reset)();
};
