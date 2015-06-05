#pragma once

namespace VersionOne {
    namespace TestComplete {
        namespace ManagedWrapper {

            __declspec(dllexport) enum EventType {
                Start = 0,
                Stop = 1,
				Fail = 2,
            };
        }
    }
}