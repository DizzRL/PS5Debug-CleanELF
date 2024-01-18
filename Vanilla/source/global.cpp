#include "global.h"

#include "imports.h"
#include "utility.h"

uint64_t processBaseAddress = 0;

int executionID = 0;
uint64_t executionAddress = 0;

int userId = -1;
int userPadHandle = -1;

bool ShouldRun() {
	if (executionAddress == 0)
		return false;

	if (executionID == *(int*)(executionAddress))
		return true;
	else
		return false;
}

void NewRunID() {
	executionID = *(int*)(executionAddress)+1;
	*(int*)executionAddress = executionID;
}