#pragma once

#include "stdafx.h"
#include <windows.h>

#include "EventType.h"

namespace VersionOne {
	namespace TestComplete {
		namespace ManagedWrapper {

			class RawDataWrapper
			{
			private:
				void* rawData;				

			public:
				__declspec(dllexport) RawDataWrapper();
				virtual __declspec(dllexport) ~RawDataWrapper();

				BSTR __declspec(dllexport) get_TestName();
				void __declspec(dllexport) set_TestName(BSTR value);

				BSTR __declspec(dllexport) get_ProjectName();
				void __declspec(dllexport) set_ProjectName(BSTR value);

				EventType __declspec(dllexport) get_EventType();
				void __declspec(dllexport) set_EventType(EventType value);

				void* GetWrappedObject();
			};

		}
	}
}
