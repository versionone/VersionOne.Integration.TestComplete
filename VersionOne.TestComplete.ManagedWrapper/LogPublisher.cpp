#include "StdAfx.h"
#include "LogPublisher.h"
#include "ComponentRepository.h"
#include "TypeConverter.h"

using namespace VersionOne::TestComplete::ManagedWrapper;

LogPublisher::LogPublisher() {
}

LogPublisher::~LogPublisher() {
}

void LogPublisher::Fatal(BSTR message){
    String^ convertedMessage = TypeConverter::ConvertStringToManaged(message);

    Logger^ logger = ComponentRepository::Instance->GetLogger();
    logger->Fatal(convertedMessage);
}

void LogPublisher::Info(BSTR message){
    String^ convertedMessage = TypeConverter::ConvertStringToManaged(message);

    Logger^ logger = ComponentRepository::Instance->GetLogger();
    logger->Info(convertedMessage);
}

void LogPublisher::Info(std::wstring message){
    String^ convertedMessage = TypeConverter::ConvertStringToManaged(message);

    Logger^ logger = ComponentRepository::Instance->GetLogger();
    logger->Info(convertedMessage);
}

void LogPublisher::Debug(BSTR message){
    String^ convertedMessage = TypeConverter::ConvertStringToManaged(message);

    Logger^ logger = ComponentRepository::Instance->GetLogger();
    logger->Debug(convertedMessage);
}

void LogPublisher::Debug(std::wstring message){
    String^ convertedMessage = TypeConverter::ConvertStringToManaged(message);

    Logger^ logger = ComponentRepository::Instance->GetLogger();
    logger->Debug(convertedMessage);
}