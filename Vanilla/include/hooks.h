#pragma once

#include "global.h"

NAMESPACE(Hooks)

typedef void(*SomeHook_t)(int some_param);

extern SomeHook_t SomeHook_s;

void SomeHook(int some_param);

END
