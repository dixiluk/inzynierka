#include "CrashReporter.h"
#include <stdio.h>
#include <stdlib.h>

void CrashReporter::ErrorMessageM(const char* message, const char* element){
	printf("Error in '%s': %s\n", element, message);
	exit(-1);
}

void CrashReporter::TriggerCrashM(const char* message, const char* element){
	printf("Crash detected in '%s': %s\n", element, message);
	exit(-1);
}

void CrashReporter::WarningMessageM(const char* message, const char* element){
	printf("Warning in '%s': %s\n", element, message);
}