#pragma once

#include "EventCollector.h"
#include "LogPublisher.h"

using namespace VersionOne::TestComplete::ManagedWrapper;

class ATL_NO_VTABLE CProjectItem : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CProjectItem>,
	public ItcProjectNodeInternal,
	public ItcProjectItem,
	public ItcProjectContextItem,
	public IaqEventsListenerProvider,
	public ItcTestEngineEvent,
    public ItcLogMessageEvent
{
	BEGIN_COM_MAP(CProjectItem)
		COM_INTERFACE_ENTRY2(ItcProjectNode, ItcPersistentProjectNode)
		COM_INTERFACE_ENTRY (ItcPersistentProjectNode)
		COM_INTERFACE_ENTRY (ItcProjectNodeInternal)
		COM_INTERFACE_ENTRY (ItcProjectItem)
		COM_INTERFACE_ENTRY (ItcProjectContextItem)
		COM_INTERFACE_ENTRY2(IaqBaseEvent, IaqEventsListenerProvider)
		COM_INTERFACE_ENTRY (IaqEventsListenerProvider)
		COM_INTERFACE_ENTRY (ItcTestEngineEvent)
        COM_INTERFACE_ENTRY (ItcLogMessageEvent)
	END_COM_MAP()

public:
	CProjectItem();
	void Initialize(GUID key, ItcProjectNode* owner, IaqMessenger* messenger, IaqCoreUI* coreUI, IaqEventManager* eventManager);

    // ItcProjectContextItem Methods
	STDMETHOD_(void, GetContextKey)(GUID * value);

	// ItcProjectNodeInternal Methods
	STDMETHOD(Initialize)(IaqBaseManager * baseManager);
	STDMETHOD(Finalize)();
	STDMETHOD(DeletePermanently)();
	STDMETHOD(QueryClose)(VARIANT_BOOL * value);

	// ItcPersistentProjectNode Methods
	STDMETHOD(get_Dirty)(VARIANT_BOOL * value);
	STDMETHOD(put_Dirty)(VARIANT_BOOL value);
	STDMETHOD(Save)(AQ_SAVE_RESULT * value);
	STDMETHOD(InitializeFromPersistentInfo)(IaqOptionsNode * persistentInfo, VARIANT_BOOL * value);
	STDMETHOD(StorePersistentInfo)(IaqOptionsNode * persistentInfo);

	// ItcProjectNode Methods
	STDMETHOD(get_Caption)(BSTR * value);
	STDMETHOD(put_Caption)(BSTR value);
	STDMETHOD(get_NodeType)(GUID * value);
	STDMETHOD(get_NodeProperty)(TC_PROJECT_NODE_PROP propID, VARIANT * value);
	STDMETHOD(put_NodeProperty)(enum TC_PROJECT_NODE_PROP propID, VARIANT value);
	STDMETHOD(GetNodeGUIDProperty)(enum TC_PROJECT_NODE_GUIDPROP propID, GUID * value);
	STDMETHOD(SetNodeGUIDProperty)(enum TC_PROJECT_NODE_GUIDPROP propID, GUID * value);
	STDMETHOD(get_OwnerNode)(ItcProjectNode * * value);
	STDMETHOD(CreateChildIterator)(ItcProjectNodeIterator * * value);
	STDMETHOD(get_HasTests)(VARIANT_BOOL * value);
	STDMETHOD(GetTest)(BSTR moniker, ItcTest * * value);
	STDMETHOD(EnumTest)(ItcTestIterator * * value);

	// IaqKeyItem Methods
	STDMETHOD_(void, GetKey)(GUID * value);

	// IaqBaseEvent Methods
	STDMETHOD(get_EventInfo)(BSTR * value);

	// IaqEventsListenerProvider Methods
	STDMETHOD_(long, GetCountListenedEvent)();
	STDMETHOD_(void, GetListenedEventInfo)(long index, AQ_EVENT_INFO * value);

	// ItcTestEngineEvent Methods
	STDMETHOD(OnTestEvent)(TC_TESTENGINE_EVENT eventInfo, ItcTest * test);
	STDMETHOD(OnEngineEvent)(TC_TESTENGINE_EVENT eventInfo);

	// ItcLogMessageEvent Methods
	STDMETHOD_(void, OnLogMessage)(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, VARIANT_BOOL * Locked);
	STDMETHOD_(void, OnLogWarning)(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, VARIANT_BOOL * Locked);
	STDMETHOD_(void, OnLogError)(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, VARIANT_BOOL * Locked);
	STDMETHOD_(void, OnLogEvent)(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, VARIANT_BOOL * Locked);
	STDMETHOD_(void, OnLogPicture)(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, BSTR * DestPictureFileName, VARIANT_BOOL * Locked);
	STDMETHOD_(void, OnLogFile)(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, BSTR * SrcFileName, BSTR * DestFileName, VARIANT_BOOL * Locked);
	STDMETHOD_(void, OnLogLink)(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, BSTR * Link, VARIANT_BOOL * Locked);
	STDMETHOD_(void, OnLogCreateNode)(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, VARIANT_BOOL * Locked);
	STDMETHOD_(void, OnLogCloseNode)(BSTR * Message, BSTR * MessageEx, long * Priority, VARIANT_BOOL * Locked);
	STDMETHOD_(void, OnLogCheckpoint)(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, VARIANT_BOOL * Locked);
    
    HRESULT InitializeFromConfiguration(CComPtr<ItcProjectNodeConfigurationInfo> configuration);
	HRESULT InitializeFromFile(CONST TCHAR* fileName, CComBSTR caption);

private:
	HRESULT Load();
    bool IsSupportedTestType(GUID testType);
    std::wstring GetFormattedString(BSTR s1, BSTR s2, BSTR s3, BSTR s4, BSTR s5);

	CComBSTR itemCaption;
	TCHAR itemFileName[MAX_PATH];
	CComPtr<ItcProjectNode> itemOwner;
	GUID itemKey;
	CComBSTR itemText;
	GUID itemContextKey;
	HBITMAP itemBitmap;
	HBITMAP itemPersonalBitmap;
	CComVariant itemSccStatus;
	CComBSTR itemHint;
	CComPtr<IaqMessenger> itemMessenger;
	CComPtr<IaqCoreUI> itemCoreUI;
	CComPtr<IaqEventManager> itemEventManager;
    long itemGeneration;
	long itemCommittedGeneration;
    EventCollector* eventCollector;
    BSTR projectName;
    BSTR suiteName;
    LogPublisher* logger;
};
