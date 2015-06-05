#pragma once

#include "stdafx.h"
#include <windows.h>
#include <sstream>

namespace VersionOne {
    namespace TestComplete {
        namespace ManagedWrapper {
            
            class LogPublisher
            {
            public:
                __declspec(dllexport) LogPublisher(void);
                virtual __declspec(dllexport) ~LogPublisher(void);

                void __declspec(dllexport) Fatal(BSTR message);

                void __declspec(dllexport) Info(BSTR message);
                void __declspec(dllexport) Info(std::wstring message);

                void __declspec(dllexport) Debug(BSTR message);
                void __declspec(dllexport) Debug(std::wstring message);
            };

        }
    }
}