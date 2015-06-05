// SamplePlugIn.cpp : Implementation of CPlugin

#include "stdafx.h"
#include "ProjectItem.h"
#include "ProjectItemInfo.h"
#include "Plugin.h"

// CPlugin

CPlugin::CPlugin() {
	m_project_node_info = NULL;
}

CONST GUID CPlugin::GetPluginKey() {
    // {B620AB62-7FBA-4b52-A7A8-C619608332CB}
    CONST GUID key = {0xb620ab62, 0x7fba, 0x4b52, { 0xa7, 0xa8, 0xc6, 0x19, 0x60, 0x83, 0x32, 0xcb}};
	return key;
}

HRESULT CPlugin::CreatePlugin(IaqBasePlugin * * Value) {
	_ASSERT( Value != NULL );

	CComObject<CPlugin>* p;
	HRESULT hr = CComObject<CPlugin>::CreateInstance(&p);
	if (FAILED(hr)) return hr;

	return p->QueryInterface(Value);
}


// IaqBasePlugin Methods

HRESULT STDMETHODCALLTYPE CPlugin::Initialize(IaqBaseManager * Manager) {
	HRESULT hr;

	CComPtr<IaqSubsystemManager> subsystem_manager;

    //Messenger
	hr = Manager->get_Managers(IID_IaqMessenger, &subsystem_manager);
	if (FAILED(hr)) return hr;

	hr = subsystem_manager->QueryInterface(&m_messenger);
	if (FAILED(hr)) return hr;
	subsystem_manager = NULL;

    //Core UI
	hr = Manager->get_Managers(IID_IaqCoreUI, &subsystem_manager);
	if (FAILED(hr)) return hr;

	hr = subsystem_manager->QueryInterface(&m_core_ui);
	if (FAILED(hr)) return hr;
	subsystem_manager = NULL;

    //Event Manager
	hr = Manager->get_Managers(IID_IaqEventManager, &subsystem_manager);
	if (FAILED(hr)) return hr;

	hr = subsystem_manager->QueryInterface(&m_event_manager);
	if (FAILED(hr)) return hr;
	subsystem_manager = NULL;

    CComObject<CProjectItemInfo>* p;
	hr = CComObject<CProjectItemInfo>::CreateInstance(&p);
	if (FAILED(hr)) return hr;

	p->Initialize(m_messenger);

	return p->QueryInterface(&m_project_node_info);
}


HRESULT STDMETHODCALLTYPE CPlugin::Update()
{
	return S_OK;
}


HRESULT STDMETHODCALLTYPE CPlugin::Finalize()
{
	m_project_node_info = NULL;
	m_messenger         = NULL;
	m_core_ui           = NULL;
	m_event_manager     = NULL;
	return S_OK;
}


HRESULT STDMETHODCALLTYPE CPlugin::BaseManagerStateChanged(AQ_MAINMANAGER_STATE /*State*/)
{
	return S_OK;
}


// IaqKeyItem Methods

void STDMETHODCALLTYPE CPlugin::GetKey(GUID * Value)
{
	_ASSERT( Value != NULL );
	*Value = GetPluginKey();
}


// ItcProjectNodeFactory Methods

HRESULT STDMETHODCALLTYPE CPlugin::get_ManagedProjectNodeCount(long * Value)
{
	_ASSERT( Value != NULL );
	*Value = 1;
	return S_OK;
}


HRESULT STDMETHODCALLTYPE CPlugin::CreateManagedProjectNode(GUID * /*ProjectNodeType*/, GUID * ProjectNodeKey, ItcProjectNodeConfigurationInfo * Configuration, IaqBaseManager * ABaseManager, ItcProjectNode * AOwnerNode, ItcProjectNode * * Value)
{
	CComObject<CProjectItem>* new_item;
	HRESULT hr = CComObject<CProjectItem>::CreateInstance(&new_item);
	if (FAILED(hr)) return hr;

	new_item->Initialize(*ProjectNodeKey, AOwnerNode, m_messenger, m_core_ui, m_event_manager);
	new_item->Initialize(ABaseManager);

	if (Configuration != NULL)
		new_item->InitializeFromConfiguration(Configuration);

	return new_item->QueryInterface(Value);
}


HRESULT STDMETHODCALLTYPE CPlugin::CreateProjectNodeFromStorage(IaqBaseManager * ABaseManager, ItcProjectNode * AOwnerNode, BSTR StorageName, ItcProjectNode * * Value)
{
	_ASSERT( Value != NULL );

	HRESULT hr;

	// Get the node name from the file name
	CComBSTR node_name = GetCaptionFromFileName(_bstr_t(StorageName));
	// Check whether the node can be added
	if (NodeExistsInParent(AOwnerNode, node_name)) return ERROR_BAD_ARGUMENTS;

	// Generate the session-independent GUID
	GUID new_key;
	hr = CoCreateGuid(&new_key);
	if (FAILED(hr)) return hr;

	// Create the node
	CComObject<CProjectItem>* new_item;
	hr = CComObject<CProjectItem>::CreateInstance(&new_item);
	if (FAILED(hr)) return hr;

	new_item->Initialize(new_key, AOwnerNode, m_messenger, m_core_ui, m_event_manager);

	// Initialize the node with the IaqBaseManager object
	hr = new_item->Initialize(ABaseManager);
	if (FAILED(hr)) return hr;

	// Read node's data from a file
	hr = new_item->InitializeFromFile(_bstr_t(StorageName), node_name);
	if (FAILED(hr)) return hr;

	return new_item->QueryInterface(Value);
}


HRESULT STDMETHODCALLTYPE CPlugin::GetManagedProjectNodeInfo(long ManagedNodeIndex, ItcProjectNodeInfo * * Value)
{
	_ASSERT( Value != NULL );
	_ASSERT( ManagedNodeIndex == 0 );

	if (ManagedNodeIndex == 0)
		return m_project_node_info.CopyTo(Value);
	else
		return ERROR_BAD_ARGUMENTS;
}
