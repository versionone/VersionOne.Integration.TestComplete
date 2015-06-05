#include "stdafx.h"
#include "ProjectItem.h"
#include "ProjectItemInfo.h"
#include "ChildNodeIterator.h"
#include "RawDataWrapper.h"

#include <sstream>

using namespace std;
using namespace VersionOne::TestComplete::ManagedWrapper;

CProjectItem::CProjectItem() {
	itemCaption = L"";
	itemFileName[0] = 0;
	itemKey = GUID_NULL;
	itemText = L"";
	CoCreateGuid(&itemContextKey);
	itemBitmap = NULL;
	itemPersonalBitmap = NULL;
	itemSccStatus = -1;
	itemHint = L"";
}

void CProjectItem::Initialize(GUID key,
                              ItcProjectNode* owner,
                              IaqMessenger* messenger,
                              IaqCoreUI* coreUI,
                              IaqEventManager* eventManager) {
	itemKey = key;
	itemOwner = owner;
	itemMessenger = messenger;
	itemCoreUI = coreUI;
	itemEventManager = eventManager;

	eventCollector = new EventCollector();
    logger = new LogPublisher();

    HRESULT hr;
    hr = itemOwner->get_Caption(&projectName);

    ItcProjectNode* projectSuite;
    hr = itemOwner->get_OwnerNode(&projectSuite);
    
	if(SUCCEEDED(hr)) {
        hr = projectSuite->get_Caption(&suiteName);
    }

    logger->Info(_T("VersionOne plugin initialized."));
}

// ItcProjectContextItem Methods
void STDMETHODCALLTYPE CProjectItem::GetContextKey(GUID * value) {
	_ASSERT(value != NULL);
	*value = itemContextKey;
}

// ItcProjectNodeInternal Methods
HRESULT STDMETHODCALLTYPE CProjectItem::Initialize(IaqBaseManager * /*BaseManager*/) {
	_ASSERT(itemKey != GUID_NULL);
	_ASSERT(itemOwner!= NULL);
	_ASSERT(itemMessenger != NULL);
	_ASSERT(itemCoreUI != NULL);
	_ASSERT(itemEventManager != NULL);

	CComPtr<IaqEventsListenerProvider> elp;
	QueryInterface(IID_IaqEventsListenerProvider, (void**)&elp);
	itemEventManager->AddEventsProvider(elp);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProjectItem::Finalize() {
	_ASSERT(itemEventManager != NULL);
	_ASSERT(itemOwner != NULL);

	GUID key;
	GetKey(&key);

	itemEventManager->RemoveEventsProvider(&key);

	itemOwner = NULL;
	itemMessenger = NULL;
	itemCoreUI = NULL;
	itemEventManager = NULL;

	if(itemBitmap != NULL) {
		DeleteObject(itemBitmap);
		itemBitmap = NULL;
	}

	if(itemPersonalBitmap != NULL) {
		DeleteObject(itemPersonalBitmap);
		itemPersonalBitmap = NULL;
	}

    logger->Info(_T("VersionOne plugin finalized."));

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProjectItem::DeletePermanently() {
	Finalize();
	DeleteFile(itemFileName);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProjectItem::QueryClose(VARIANT_BOOL * value) {
	_ASSERT(value != NULL);
	*value = VARIANT_TRUE;

	return S_OK;
}

// ItcPersistentProjectNode Methods
HRESULT STDMETHODCALLTYPE CProjectItem::get_Dirty(VARIANT_BOOL * value) {
	_ASSERT(value != NULL);
	*value = (itemCommittedGeneration == itemGeneration) ? VARIANT_FALSE : VARIANT_TRUE;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProjectItem::put_Dirty(VARIANT_BOOL value) {
	if (value) {
		itemGeneration ++;
	} else {
		itemCommittedGeneration = itemGeneration;
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProjectItem::Save(AQ_SAVE_RESULT * value) {
	_ASSERT(value != NULL);
	*value = AQ_SR_CANCEL;

	HRESULT hr = ForceDirectoriesForFile(itemFileName);
    if (FAILED(hr)) {
        return hr;
    }

	FILE* f = _tfopen(itemFileName, _T("wb"));
    if (f == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }

	CW2A buf(itemText);
	size_t written = fwrite(buf, 1, itemText.Length(), f);

	fclose(f);

    if (written != itemText.Length()) {
        return STG_E_WRITEFAULT;
    }

	put_Dirty(VARIANT_FALSE);
	*value = AQ_SR_OK;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProjectItem::InitializeFromPersistentInfo(IaqOptionsNode * persistentInfo, VARIANT_BOOL * value) {
	_ASSERT(value != NULL);
	_ASSERT(persistentInfo != NULL);
	*value = VARIANT_FALSE;

	CComPtr<IaqProperties> props;
	persistentInfo->GetProperties(&props);

	itemFileName[0] = 0;
	_bstr_t fn = itemFileName;
	HRESULT hr = props->GetString(CComBSTR(cFileNameProperty), CComBSTR(L""), fn.GetAddress());
    if (FAILED(hr)) {
        return hr;
    }

	_tcsncpy(itemFileName, fn, MAX_PATH);  itemFileName[MAX_PATH - 1] = 0;

	itemGeneration = 0;
	put_Dirty(VARIANT_FALSE);

	if (itemFileName[0] != 0) {
		itemCaption = GetCaptionFromFileName(itemFileName);

		hr = Load();
        if (FAILED(hr)) {
            return hr;
        }

		*value = VARIANT_TRUE;
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProjectItem::StorePersistentInfo(IaqOptionsNode * persistentInfo) {
	HRESULT hr;

	CComPtr<IaqProperties> props;
	persistentInfo->GetProperties(&props);

	hr = props->SetString(CComBSTR(cFileNameProperty), CComBSTR(itemFileName));
    if (FAILED(hr)) {
        return hr;
    }

	return S_OK;
}

// ItcProjectNode Methods
HRESULT STDMETHODCALLTYPE CProjectItem::get_Caption(BSTR * value) {
	_ASSERT(value != NULL);
	return itemCaption.CopyTo(value);
}


HRESULT STDMETHODCALLTYPE CProjectItem::put_Caption(BSTR value) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProjectItem::get_NodeType(GUID * value) {
	_ASSERT(value != NULL);
	*value = CProjectItemInfo::GetKey();

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProjectItem::get_NodeProperty(TC_PROJECT_NODE_PROP propID, VARIANT * value) {
	_ASSERT(value != NULL);

	CComVariant val;
	val.Attach(value);

	switch(propID) {
		case pnpStorageName:
			// We return the file name, since our project item is a persistent node
			val = itemFileName;
			break;
		case pnpDescription:
			// No additional info
			val = L"";
			break;
		case pnpIsReadOnly:
			// The node is read-only
			val = true;
			break;
		case pnpSettingsStorage:
			// We don't have a Settings Storage
			break;
		case pnpPersonalBitmap:
            if (itemPersonalBitmap != NULL) {
				val = (long)CopyImage(itemPersonalBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
            }
			break;
		case pnpHint:
			// No hint
			val = L"";
			break;
		case pnpSCCStatus:
			val = itemSccStatus;
			break;
		case pnpCanDeletePermanently:
			val = true;
			break;
		default:
			_ASSERT(false);
			return E_INVALIDARG;
	}

	return val.Detach(value);
}

HRESULT STDMETHODCALLTYPE CProjectItem::put_NodeProperty(TC_PROJECT_NODE_PROP propID, VARIANT value) {
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CProjectItem::GetNodeGUIDProperty(TC_PROJECT_NODE_GUIDPROP propID, GUID * value) {
	_ASSERT(value != NULL);
	
	switch(propID) {
		case pngpContextKey:
			*value = itemContextKey;
			return S_OK;
		default:
			_ASSERT(false);
			return S_OK;
	}
}

HRESULT STDMETHODCALLTYPE CProjectItem::SetNodeGUIDProperty(TC_PROJECT_NODE_GUIDPROP /*propID*/, GUID * /*value*/) {
	return E_NOTIMPL;
}


HRESULT STDMETHODCALLTYPE CProjectItem::get_OwnerNode(ItcProjectNode * * value) {
	_ASSERT(value != NULL);

	return itemOwner.CopyTo(value);
}

HRESULT STDMETHODCALLTYPE CProjectItem::CreateChildIterator(ItcProjectNodeIterator * * value) {
	_ASSERT(value != NULL);
	CComObject<CChildNodeIterator>* iterator;
	HRESULT hr = CComObject<CChildNodeIterator>::CreateInstance(&iterator);
    if (FAILED(hr)) {
        return hr;
    }

	return iterator->QueryInterface(value);
}

HRESULT STDMETHODCALLTYPE CProjectItem::get_HasTests(VARIANT_BOOL * value) {
	_ASSERT(value != NULL);
	*value = VARIANT_FALSE;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProjectItem::GetTest(BSTR /*Moniker*/, ItcTest * * value) {
	_ASSERT(value != NULL);
	*value = NULL;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CProjectItem::EnumTest(ItcTestIterator * * value) {
	_ASSERT(value != NULL);
	*value = NULL;

	return S_OK;
}

// IaqKeyItem Methods
void STDMETHODCALLTYPE CProjectItem::GetKey(GUID * value) {
	_ASSERT(value != NULL);
	*value = itemKey;
}

HRESULT CProjectItem::InitializeFromConfiguration(CComPtr<ItcProjectNodeConfigurationInfo> configuration) {
	if (configuration != NULL) {
		HRESULT hr;

		CComVariant configCaption;
		hr = configuration->get_ParameterValue(0, &configCaption);
        if (FAILED(hr)) {
            return hr;
        }

		hr = configCaption.CopyTo(&itemCaption);
        if (FAILED(hr)) {
            return hr;
        }

		CComVariant path;
		hr = configuration->get_ParameterValue(1, &path);
        if (FAILED(hr)) {
            return hr;
        }

        if (path.vt != VT_BSTR) {
            return ERROR_BAD_ARGUMENTS;
        }

		_bstr_t fileName = path.bstrVal + _bstr_t(itemCaption) + "." + cDefaultFileExtension;

		_tcsncpy(itemFileName, fileName, MAX_PATH);
		itemFileName[MAX_PATH - 1] = 0;
	}

	return S_OK;
}

HRESULT CProjectItem::InitializeFromFile(CONST TCHAR* fileName, CComBSTR fileCaption) {
	itemCaption = fileCaption;
	_tcsncpy(itemFileName, fileName, MAX_PATH);
	itemFileName[MAX_PATH - 1] = 0;

	HRESULT hr = Load();
    if (FAILED(hr)) {
        return hr;
    }

	itemGeneration = 0;
	put_Dirty(VARIANT_FALSE);

	return S_OK;
}

HRESULT CProjectItem::Load() {
	itemText = L"";

	FILE* f = _tfopen(itemFileName, _T("rb"));
    if (f == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }

	size_t len = _filelength(_fileno(f));
	char* buf = (char*)malloc((len + 1) * sizeof(char));

	size_t read = fread(buf, 1, len, f);
	fclose(f);

	if (read != len) {
		free(buf);
		return STG_E_READFAULT;
	}

	buf[len] = 0;
	itemText = CComBSTR(buf);
	free(buf);

	return S_OK;
}

// IaqBaseEvent Methods
HRESULT STDMETHODCALLTYPE CProjectItem::get_EventInfo(BSTR * value) {
	_ASSERT(value != NULL);
	CComBSTR(cClassName).CopyTo(value);

	return S_OK;
}

// IaqEventsListenerProvider Methods
long STDMETHODCALLTYPE CProjectItem::GetCountListenedEvent() {
	return 2;
}

void STDMETHODCALLTYPE CProjectItem::GetListenedEventInfo(long index, AQ_EVENT_INFO * value) {
	_ASSERT(value != NULL);

    switch(index) {
        case 0:
            value->EventID = IID_ItcTestEngineEvent;
            break;

        case 1:
            value->EventID = IID_ItcLogMessageEvent;
            break;

        default:
            return;
    }

    value->NotifyInMainThread = VARIANT_FALSE;
}

// ItcTestEngineEvent Methods
HRESULT STDMETHODCALLTYPE CProjectItem::OnTestEvent(TC_TESTENGINE_EVENT eventInfo, ItcTest * Test) {
    GUID testType;
    Test->GetType(&testType);

    if(!IsSupportedTestType(testType)) {
        return S_OK;
    }

    BSTR testName;
    Test->GetName(&testName);

    wstring message;

    switch (eventInfo) {
		case TC_PLAYING_START: {
            message = GetFormattedString(_T(" Test "), projectName, _T("."), testName, _T(" started."));
            logger->Info(message);

            RawDataWrapper* rawData = new RawDataWrapper();
            rawData->set_EventType(Start);
            rawData->set_ProjectName(projectName);
            rawData->set_TestName(testName);

            eventCollector->RecordEvent(rawData);
            
            delete rawData;
            
            break;
		}

		case TC_PLAYING_STOP: {
            message = GetFormattedString(_T(" Test "), projectName, _T("."), testName, _T(" stopped."));
            logger->Info(message);

            RawDataWrapper* rawData = new RawDataWrapper();
            rawData->set_EventType(Stop);
            rawData->set_ProjectName(projectName);
            rawData->set_TestName(testName);

            eventCollector->RecordEvent(rawData);
            
            delete rawData;
            
            break;
		}
	}

	return S_OK;
}

bool CProjectItem::IsSupportedTestType(GUID testType) {
    if(testType == c_TestType_ProjectSuite
        || testType == c_TestType_Project) {
            return false;
    }

    return true;
}

HRESULT STDMETHODCALLTYPE CProjectItem::OnEngineEvent(TC_TESTENGINE_EVENT eventInfo) {
    HRESULT hr = S_OK;

    switch (eventInfo) {
        case TC_PLAYING_START: {
            logger->Info(GetFormattedString(_T("Starting test suite "), suiteName, _T("."), projectName, _T("...")));

			hr = eventCollector->StartSuite(suiteName, projectName);
			
			if(FAILED(hr)) {
                logger->Fatal(_T("Failed to connect to VersionOne."));
				hr = itemMessenger->ShowSevereError(CComBSTR("Failed to connect to VersionOne"), 0, CComBSTR(_T("")));
			}
            
			break;
        }

        case TC_PLAYING_STOP: {
            hr = eventCollector->StopSuite();

			if(FAILED(hr)) {
                logger->Fatal(_T("Failed to publish test run results to VersionOne."));
				hr = itemMessenger->ShowSevereError(CComBSTR("Failed to publish test run results to VersionOne"), 0, CComBSTR(_T("")));
			}

            logger->Info(GetFormattedString(_T("Test suite "), suiteName, _T("."), projectName, _T(" finished.")));
            break;
        }
    }

	return S_OK;
}

wstring CProjectItem::GetFormattedString(BSTR s1, BSTR s2, BSTR s3, BSTR s4, BSTR s5) {
    wstring ws1(s1);
    wstring ws2(s2);
    wstring ws3(s3);
    wstring ws4(s4);
    wstring ws5(s5);

    wstringstream str;
    str << ws1 << ws2 << ws3 << ws4 << ws5;

    wstring result = str.str();

    return result;
}

// ItcLogMessageEvent members
void STDMETHODCALLTYPE CProjectItem::OnLogMessage(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, VARIANT_BOOL * Locked) {
    logger->Debug(GetFormattedString(_T("OnLogMessage event: "), *Message, _T(""), _T(""), _T("")));
}

void STDMETHODCALLTYPE CProjectItem::OnLogWarning(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, VARIANT_BOOL * Locked) {
    logger->Debug(GetFormattedString(_T("OnLogWarning event: "), *Message, _T(""), _T(""), _T("")));
}

void STDMETHODCALLTYPE CProjectItem::OnLogError(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, VARIANT_BOOL * Locked) {
    logger->Debug(GetFormattedString(_T("OnLogError event: "), *Message, _T(""), _T(""), _T("")));
    eventCollector->FailCurrentTest();
}

void STDMETHODCALLTYPE CProjectItem::OnLogEvent(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, VARIANT_BOOL * Locked) {
    logger->Debug(GetFormattedString(_T("OnLogEvent event: "), *Message, _T(""), _T(""), _T("")));
}

void STDMETHODCALLTYPE CProjectItem::OnLogPicture(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, BSTR * DestPictureFileName, VARIANT_BOOL * Locked) {
    logger->Debug(GetFormattedString(_T("OnLogPicture event: "), *Message, _T(""), _T(""), _T("")));
}

void STDMETHODCALLTYPE CProjectItem::OnLogFile(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, BSTR * SrcFileName, BSTR * DestFileName, VARIANT_BOOL * Locked) {
    logger->Debug(GetFormattedString(_T("OnLogFile event: "), *Message, _T(""), _T(""), _T("")));
}

void STDMETHODCALLTYPE CProjectItem::OnLogLink(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, BSTR * Link, VARIANT_BOOL * Locked) {
    logger->Debug(GetFormattedString(_T("OnLogLink event: "), *Message, _T(""), _T(""), _T("")));
}

void STDMETHODCALLTYPE CProjectItem::OnLogCreateNode(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, VARIANT_BOOL * Locked) {
    logger->Debug(GetFormattedString(_T("OnLogCreateNode event: "), *Message, _T(""), _T(""), _T("")));
}

void STDMETHODCALLTYPE CProjectItem::OnLogCloseNode(BSTR * Message, BSTR * MessageEx, long * Priority, VARIANT_BOOL * Locked) {
    logger->Debug(GetFormattedString(_T("OnLogCloseNode event: "), *Message, _T(""), _T(""), _T("")));
}

void STDMETHODCALLTYPE CProjectItem::OnLogCheckpoint(BSTR * Message, BSTR * MessageEx, long * Priority, struct TC_DATAROW_ATTRIB * Attrib, VARIANT_BOOL * Locked) {
    logger->Debug(GetFormattedString(_T("OnLogCheckpoint event: "), *Message, _T(""), _T(""), _T("")));
}
