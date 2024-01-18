#include "Syscall.h"

unsigned long syscall(unsigned int n, ...) {
    asm(".intel_syntax noprefix");
    asm("xor rax, rax");
    asm("mov r10, rcx");
    asm("jmp qword ptr [rip + ptr_syscall]");
    asm("ret");
}