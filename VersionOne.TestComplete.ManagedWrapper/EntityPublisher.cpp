#include "StdAfx.h"
#include "EntityPublisher.h"
#include "TypeConverter.h"
#include "EventType.h"
#include "RawData.h"
#include "ComponentRepository.h"

using namespace System;
using namespace VersionOne::TestComplete::ManagedWrapper;
using namespace VersionOne::TestComplete::V1Connector;

EntityPublisher::EntityPublisher() { }

EntityPublisher::~EntityPublisher() { }

HRESULT EntityPublisher::CreateSuite(String^ suiteName, String^ projectName) {
	suite = gcnew SuiteResult();
	String^ reference = suiteName + "." + projectName;
	suite->Reference = reference;
	suite->Name = reference;

	try {
        writer = ComponentRepository::Instance->GetWorkitemWriter();
	} catch(WorkitemWriterException^) {
		return S_FALSE;
	}

	return S_OK;
}

void EntityPublisher::CreateTest(RawData^ data) {
	TestResult^ test = gcnew TestResult();
	test->Reference = data->ProjectName + "." + data->TestName;
    suite->TestResults->Add(test);
}

void EntityPublisher::UpdateSuite() {
	suite->CalculateStatistics();
}

void EntityPublisher::UpdateTest(RawData^ data) {
    if(suite->TestResults->Count == 0) {
        return;
    }

	TestResult^ test = suite->TestResults[suite->TestResults->Count - 1];
	test->RecordEndTime();

	if(test->State != TestResult::TestRunState::Failed) {
		test->State = MapState(data->EventTypeData);
	}
}

void EntityPublisher::FailCurrentTest() {
	if(suite == nullptr || suite->TestResults->Count == 0) {
		return;
	}

	TestResult^ currentTest = suite->TestResults[suite->TestResults->Count - 1];
    TestResult::TestRunState currentState = currentTest->State;

    if(currentState == TestResult::TestRunState::Failed) {
        return;
    }

	currentTest->State = TestResult::TestRunState::Failed;

    String^ errorMessage = String::Format("Test {0} failed", currentTest->Reference);

    Logger^ logger = ComponentRepository::Instance->GetLogger();
    logger->Info(errorMessage);
}

HRESULT EntityPublisher::Publish() {
    try {
        writer->Publish(suite);
    } catch (WorkitemWriterException^ ex) {
        return S_FALSE;
    }

    return S_OK;
}

TestResult::TestRunState EntityPublisher::MapState(EventType eventType) {
	switch(eventType) {
		case Fail:
			return TestResult::TestRunState::Failed;
		
		default:
			return TestResult::TestRunState::Passed;
	}
}