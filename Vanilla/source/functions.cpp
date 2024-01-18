#include "functions.h"
#include "global.h"

NAMESPACE(Functions)

Some_Function_t Some_Function;

void Init() {
	Some_Function = (Some_Function_t)CALC_ADDRESS(0x000000);
}


END
