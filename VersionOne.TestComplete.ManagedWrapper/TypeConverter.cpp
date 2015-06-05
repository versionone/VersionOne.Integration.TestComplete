#include "stdafx.h"
#include "TypeConverter.h"

#include <msclr/marshal.h>
#include <msclr/marshal_atl.h>

#include <strsafe.h>
#include <sstream>

#include "RawData.h"

using namespace System;
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;
using namespace VersionOne::TestComplete::ManagedWrapper;

void TypeConverter::ToBSTR(String^ s, wchar_t buffer[], std::size_t sz, BSTR s1) {
    void* temp = Marshal::StringToHGlobalAnsi(s).ToPointer();
    errno_t result = wcscpy_s(buffer, sz, (const wchar_t*) temp);
	Marshal::FreeHGlobal(IntPtr(temp));
	s1 = buffer;
}

// TODO handle arbitrary string length
BSTR TypeConverter::ConvertString(String^ s) {
    wchar_t buffer[100];
    BSTR result = NULL;

	TypeConverter::ToBSTR(s, buffer, sizeof(buffer), result);

    return result;
}

String^ TypeConverter::ConvertStringToManaged(BSTR source) {
	return marshal_as<String^>(source);
}

Object^ TypeConverter::ConvertToManagedType(void *wrappedObject) {
    GCHandle h = GCHandle::FromIntPtr(IntPtr(wrappedObject));
    return h.Target;
}

RawData^ TypeConverter::GetRawDataFromPointer(void* wrappedObject) {
	GCHandle h = GetHandle(wrappedObject);
    return safe_cast<RawData^>(h.Target);
}

String^ TypeConverter::ConvertStringToManaged(std::wstring source) {
    return marshal_as<String^>(source.c_str());
}

EntityPublisher^ TypeConverter::GetEntityPublisherFromPointer(void* wrappedObject) {
    GCHandle h = GetHandle(wrappedObject);
	return safe_cast<EntityPublisher^>(h.Target);
}

GCHandle TypeConverter::GetHandle(void* wrappedObject) {
    return GCHandle::FromIntPtr(IntPtr(wrappedObject));
}