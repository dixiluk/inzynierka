#pragma once

namespace CrashReporter
{
	void TriggerCrashM(const char* message, const char* element);
	void ErrorMessageM(const char* message, const char* element);
	void WarningMessageM(const char* message, const char* element);
};

#define ReportError(message) CrashReporter::ErrorMessageM(message, __FUNCTION__)
#define ReportWarning(message) CrashReporter::WarningMessageM(message, __FUNCTION__)
#define TriggerCrash(message) CrashReporter::TriggerCrashM(message, __FUNCTION__)