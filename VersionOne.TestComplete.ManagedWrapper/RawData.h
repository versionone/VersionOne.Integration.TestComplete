#pragma once

#include "stdafx.h"
#include "EventType.h"

namespace VersionOne {
    namespace TestComplete {
        namespace ManagedWrapper {

			ref class RawData
			{
			private:
				System::String^ testName;
				System::String^ projectName;
				EventType eventType;

			public:
				property System::String^ RawData::TestName {
					System::String^ get() {
						return testName;
					}

					void set(System::String^ value) {
						testName = value;
					}
				}

				property System::String^ RawData::ProjectName {
					System::String^ get() {
						return projectName;
					}

					void set(System::String^ value) {
						projectName = value;
					}
				}

				property EventType RawData::EventTypeData {
					EventType get() {
						return eventType;
					}

					void set(EventType value) {
						eventType = value;
					}
				}
			};

		}
	}
}