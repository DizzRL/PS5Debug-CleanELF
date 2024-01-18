#pragma once

#include "types.h"

#define NAMESPACE(x) namespace x {
#define END }

#define CALC_ADDRESS(addr) (uint64_t)(addr + processBaseAddress)

extern uint64_t processBaseAddress;

extern int executionID;
extern uint64_t executionAddress;

extern int userId;
extern int userPadHandle;

bool ShouldRun();
void NewRunID();