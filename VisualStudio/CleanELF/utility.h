#pragma once

#include "types.h"

#define	VM_PROT_NONE	0x00
#define	VM_PROT_READ	0x01
#define	VM_PROT_WRITE	0x02
#define	VM_PROT_EXECUTE	0x04
#define	VM_PROT_COPY	0x08

#define	VM_PROT_ALL	(VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE)
#define VM_PROT_RW	(VM_PROT_READ | VM_PROT_WRITE)

typedef int(*_printf_t)(const char *fmt, ...);

void Sleep(unsigned int milliseconds);
void WriteJump(uint64_t address, uint64_t destination);
void ReadMemory(uint64_t address, void *buffer, int length);
void WriteMemory(uint64_t address, void *buffer, int length);
void NOP(uint64_t address, size_t length);
void uartprintf(const char *fmt, ...);
void Notify(const char *fmt, ...);
void CreateThread(void *entry, const char *name);
