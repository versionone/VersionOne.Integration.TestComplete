#include "StdAfx.h"
#include "ComponentRepository.h"

using namespace VersionOne::TestComplete::ManagedWrapper;
using namespace VersionOne::TestComplete::V1Connector;

ComponentRepository::ComponentRepository() { }

Logger^ ComponentRepository::GetLogger() {
	return gcnew Logger();
}

IWorkitemWriter^ ComponentRepository::GetWorkitemWriter() {
	if(workitemWriter == nullptr) {
		workitemWriter = gcnew WorkitemWriter();
		workitemWriter->Connect();
	}

	return workitemWriter;
}