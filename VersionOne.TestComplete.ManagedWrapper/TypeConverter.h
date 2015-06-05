#pragma once

#include <windows.h>
#include <msclr/marshal.h>
#include <msclr/marshal_atl.h>
#include <sstream>

#include "RawData.h"
#include "EntityPublisher.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace VersionOne {
	namespace TestComplete {
		namespace ManagedWrapper {

			class TypeConverter 
			{
			public:
				static void ToBSTR(String^ s, wchar_t buffer[], std::size_t sz, BSTR s1);
				static BSTR ConvertString(String^ s);
                static Object^ ConvertToManagedType(void* wrappedObject);
				static String^ ConvertStringToManaged(BSTR source);
                static String^ ConvertStringToManaged(std::wstring source);

				static RawData^ GetRawDataFromPointer(void *wrappedObject);
                static EntityPublisher^ GetEntityPublisherFromPointer(void *wrappedObject);
                static GCHandle GetHandle(void* wrappedObject);
			};
		}
	}
}