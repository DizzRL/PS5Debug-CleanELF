#pragma once
#include <stdint.h>

void kernel_init_rw(int master_sock, int victim_sock, int* rw_pipe, uint64_t pipe_addr);
int kernel_copyin(void* src, uint64_t kdest, uint64_t length);
int kernel_copyout(uint64_t ksrc, void* dest, uint64_t length);
int32_t kernel_get_ucred_caps(int pid, uint8_t caps[16]);
int32_t kernel_set_ucred_caps(int pid, uint8_t caps[16]);