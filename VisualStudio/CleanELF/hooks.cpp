#include "hooks.h"

#include "defines.h"
#include "functions.h"
#include "utility.h"

using namespace Functions;

NAMESPACE(Hooks)

SomeHook_t SomeHook_s;

void SomeHook(int some_param) {
	SomeHook_s(some_param);
	// Do More
}

END
