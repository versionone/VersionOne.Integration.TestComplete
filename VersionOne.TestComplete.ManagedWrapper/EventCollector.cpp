#include <msclr/marshal.h>
#include <msclr/marshal_atl.h>

#include "StdAfx.h"
#include "EventCollector.h"
#include "EntityPublisher.h"
#include "ComponentRepository.h"
#include "RawDataWrapper.h"
#include "TypeConverter.h"

using namespace VersionOne::TestComplete::ManagedWrapper;
using namespace System::Runtime::InteropServices;

EventCollector::EventCollector() {
    EntityPublisher^ publisher = gcnew EntityPublisher();
    this->publisherPtr = GCHandle::ToIntPtr(GCHandle::Alloc(publisher)).ToPointer();
}

EventCollector::~EventCollector() { 
    GCHandle h = TypeConverter::GetHandle(publisherPtr);
    h.Free();
}

HRESULT EventCollector::StartSuite(BSTR suiteName, BSTR projectName) {
	HRESULT hr = S_OK;

	String^ convertedSuiteName = TypeConverter::ConvertStringToManaged(suiteName);
	String^ convertedProjectName = TypeConverter::ConvertStringToManaged(projectName);

    EntityPublisher^ publisher = TypeConverter::GetEntityPublisherFromPointer(publisherPtr);
	hr = publisher->CreateSuite(convertedSuiteName, convertedProjectName);

	return hr;
}

void EventCollector::RecordEvent(RawDataWrapper* rawData) {
	RawData^ data = TypeConverter::GetRawDataFromPointer(rawData->GetWrappedObject());
    EntityPublisher^ publisher = TypeConverter::GetEntityPublisherFromPointer(publisherPtr);

	switch(data->EventTypeData) {
		case Start:
			publisher->CreateTest(data);
			break;

		case Stop:
			publisher->UpdateTest(data);
			break;
	}
}

void EventCollector::FailCurrentTest() {
    EntityPublisher^ publisher = TypeConverter::GetEntityPublisherFromPointer(publisherPtr);
	publisher->FailCurrentTest();
}

HRESULT EventCollector::StopSuite() {
    HRESULT hr = S_OK;

    EntityPublisher^ publisher = TypeConverter::GetEntityPublisherFromPointer(publisherPtr);
    publisher->UpdateSuite();
    hr = publisher->Publish();

    return hr;
}