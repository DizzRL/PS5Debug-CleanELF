#include <stdint.h>
#include <stdarg.h>

#include "defines.h"
#include "detour.h"
#include "functions.h"
#include "global.h"
#include "hooks.h"
#include "imports.h"
#include "utility.h"
#include "types.h"

void DetectGame() {
	char userName[20];
	char welcomeMessage[100];
	sceUserServiceGetUserName(userId, userName, 20);
	snprintf(welcomeMessage, sizeof(welcomeMessage), "Welcome %s to PS5Debug Clean ELF", userName);
	Notify(welcomeMessage);
	// Find a random string in the executable and compare to make sure its the correct game
	if (!strcmp((char*)CALC_ADDRESS(0x000000), "SomeString")) {
		// We are in a valid game
		executionAddress = 0x0000000; // somewhere at the end of prot 5 executable
		NewRunID();
	}
}

extern "C" void _main(struct payload_args* args) {
	dlsym_t* dlsym = args->dlsym;
	processBaseAddress = args->processBaseAddress;
	initImports(dlsym);
	// initImports MUST be called before syscalls
	initSyscall();

	SceUserServiceLoginUserIdList userIdList;
	sceUserServiceGetLoginUserIdList(&userIdList);

	for (int i = 0; i < 4; i++) {
		if (userIdList.userId[i] != -1) {
			userId = userIdList.userId[i];
			break;
		}
	}

	userPadHandle = scePadGetHandle(userId, 0, 0);

	DetectGame();
}
