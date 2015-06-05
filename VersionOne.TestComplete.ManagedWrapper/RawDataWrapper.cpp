#include "StdAfx.h"

#include <msclr/marshal.h>
#include <msclr/marshal_atl.h>

#include <strsafe.h>

#include "RawData.h"
#include "RawDataWrapper.h"
#include "TypeConverter.h"

using namespace System;
using namespace System::Runtime::InteropServices;

using namespace VersionOne::TestComplete::ManagedWrapper;

RawDataWrapper::RawDataWrapper(){
    RawData^ rawData = gcnew RawData();
	this->rawData = GCHandle::ToIntPtr(GCHandle::Alloc(rawData)).ToPointer();
}

RawDataWrapper::~RawDataWrapper() {
    GCHandle h = TypeConverter::GetHandle(rawData);
    h.Free();
}

BSTR RawDataWrapper::get_TestName() {
    RawData^ obj = TypeConverter::GetRawDataFromPointer(rawData);
	return TypeConverter::ConvertString(obj->TestName);
}

void RawDataWrapper::set_TestName(BSTR value) {
    RawData^ obj = TypeConverter::GetRawDataFromPointer(rawData);
	obj->TestName = TypeConverter::ConvertStringToManaged(value);
}

BSTR RawDataWrapper::get_ProjectName() {
    RawData^ obj = TypeConverter::GetRawDataFromPointer(rawData);
	return TypeConverter::ConvertString(obj->ProjectName);
}

void RawDataWrapper::set_ProjectName(BSTR value) {
    RawData^ obj = TypeConverter::GetRawDataFromPointer(rawData);
	obj->ProjectName = TypeConverter::ConvertStringToManaged(value);
}

EventType RawDataWrapper::get_EventType() {
    RawData^ obj = TypeConverter::GetRawDataFromPointer(rawData);
	return obj->EventTypeData;
}

void RawDataWrapper::set_EventType(EventType value) {
    RawData^ obj = TypeConverter::GetRawDataFromPointer(rawData);
	obj->EventTypeData = value;
}

void* RawDataWrapper::GetWrappedObject() {
	return rawData;
}