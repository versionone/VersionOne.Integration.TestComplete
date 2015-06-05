#pragma once

#include "stdafx.h"

#include "RawDataWrapper.h"

namespace VersionOne {
    namespace TestComplete {
        namespace ManagedWrapper {
            
            class EventCollector
            {
            private:
                void* publisherPtr;

            public:
                __declspec(dllexport) EventCollector();
                virtual __declspec(dllexport) ~EventCollector();

                HRESULT __declspec(dllexport) StartSuite(BSTR suiteName, BSTR projectName);
                void __declspec(dllexport) RecordEvent(RawDataWrapper* rawData);
				void __declspec(dllexport) FailCurrentTest();
                HRESULT __declspec(dllexport) StopSuite();
            };

        }
    }
}