#pragma once

#include <windows.h>

#include "stdafx.h"
#include "RawData.h"
#include "EventType.h"

using namespace VersionOne::TestComplete::V1Connector;

namespace VersionOne {
	namespace TestComplete {
		namespace ManagedWrapper {

			ref class EntityPublisher 
			{
			private:
				SuiteResult^ suite;
				TestResult::TestRunState MapState(EventType eventType);
                IWorkitemWriter^ writer;

			public:
				EntityPublisher();
				virtual ~EntityPublisher(void);

				HRESULT CreateSuite(System::String^ suiteName, System::String^ projectName);
				void CreateTest(RawData^ data);
				void UpdateSuite();
				void UpdateTest(RawData^ data);
				void FailCurrentTest();

				HRESULT Publish();
			};

		}
	}
}