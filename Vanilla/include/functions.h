#pragma once

#include "defines.h"
#include "global.h"
#include "imports.h"
#include "types.h"


NAMESPACE(Functions)

typedef void(*Some_Function_t)(int some_function_param);
extern Some_Function_t Some_Function;

void Init();

END
