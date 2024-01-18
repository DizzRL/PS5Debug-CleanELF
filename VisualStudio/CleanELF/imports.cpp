#include "types.h"
#include "imports.h"

void* ptr_syscall;

//kernel
int(*sceKernelDebugOutText)(int debugChannel, const char* text);
int(*sceKernelMprotect)(const void *addr, size_t len, int prot);
unsigned int(*sceKernelSleep)(unsigned int seconds);
int(*sceKernelUsleep)(unsigned int microseconds);
int (*getpid)(void);
int (*sceKernelJitCreateSharedMemory)(int flags, size_t size, int protection, int* destinationHandle);
int (*sceKernelJitCreateAliasOfSharedMemory)(int handle, int protection, int* destinationHandle);
int (*sceKernelJitMapSharedMemory)(int handle, int protection, void** destination);
size_t(*sceKernelWrite)(int32_t, const void*, size_t);
size_t(*sceKernelRead)(int32_t, void*, size_t);
int (*sysctlbyname)(const char*, void*, uint64_t*, const void*, size_t);

//threads
int (*scePthreadCreate)(ScePthread *thread, const ScePthreadAttr *attr, void *entry, void *arg, const char *name);
void (*scePthreadExit)(void *value);
int (*scePthreadDetach)(ScePthread thread);
int (*scePthreadJoin)(ScePthread thread, void **value_ptr);
int (*scePthreadGetprio)(ScePthread thread, int *prio);

//network
int (*sceNetSocket)(const char *, int, int, int);
int (*sceNetSocketClose)(int);
int (*sceNetConnect)(int, struct sockaddr *, int);
int (*sceNetSend)(int, const void *, size_t, int);
int (*sceNetSetsockopt)(int s, int level, int optname, const void* optval, uint32_t optlen);

//sys
int (*sceKernelLoadStartModule)(const char *name, size_t argc, const void *argv, unsigned int flags, int, int);

//notifications
int (*sceKernelSendNotificationRequest)(int, notify_request_t*, long, int);

//message dialogs
int (*sceCommonDialogInitialize)(void);
int (*sceMsgDialogInitialize)(void);
int (*sceMsgDialogOpen)(const SceMsgDialogParam *param);
SceCommonDialogStatus (*sceMsgDialogUpdateStatus)(void);
int (*sceMsgDialogTerminate)(void);

//ime dialogs
int (*sceImeDialogGetResult)(SceImeDialogResult *result);
int (*sceImeDialogInit)(const SceImeDialogParam *param, void *extended);
SceImeDialogStatus (*sceImeDialogGetStatus)(void);
int (*sceImeDialogTerm)(void);

//web dialogs
int (*sceWebBrowserDialogInitialize)(void);
int (*sceWebBrowserDialogTerminate)(void);
int (*sceWebBrowserDialogOpen)(const SceWebBrowserDialogParam *param);
SceCommonDialogStatus (*sceWebBrowserDialogUpdateStatus)(void);
int (*sceWebBrowserDialogSetCookie)(const SceWebBrowserDialogSetCookieParam *param);

//user service
int (*sceUserServiceGetInitialUser)(SceUserServiceUserId *userId);
int (*sceUserServiceGetUserName)(const SceUserServiceUserId userId, char *userName, const size_t size);
int (*sceUserServiceGetLoginUserIdList)(SceUserServiceLoginUserIdList *userIdList);

//gamepad
int (*scePadOpen)(SceUserServiceUserId userId, int32_t type, int32_t index, const ScePadOpenParam *pParam);
int (*scePadGetHandle)(SceUserServiceUserId userId, int32_t type, int32_t index);
int (*scePadSetLightBar)(int32_t handle, const ScePadLightBarParam *pParam);
int (*scePadReadState)(int32_t handle, ScePadData_t *pData);

//libc
int (*bzero)(void*, unsigned long);
void *(*malloc)(size_t size);
void (*free)(void *ptr);
void *(*calloc)(size_t num, size_t size);
void *(*realloc)(void *ptr, size_t size);
void *(*memset)(void *destination, int value, size_t num);
void *(*memcpy)(void *destination, const void *source, size_t num);
int (*memcmp)(const void *s1, const void *s2, size_t n);
void *(*memmove)(void *str1, const void *str2, size_t n);
size_t (*mbstowcs)(wchar_t *dest, const char *src, size_t max);
size_t (*wcstombs)(char *dest, const wchar_t *src, size_t max);
char *(*strcpy)(char *destination, const char *source);
char *(*strncpy)(char *destination, const char *source, size_t num);
char *(*strcat)(char *dest, const char *src);
char *(*strncat)(char *dest, const char *src, size_t n);
size_t (*strlen)(const char *s);
int (*strcmp)(const char *s1, const char *s2);
int (*strncmp)(const char *s1, const char *s2, size_t n);
int (*sprintf)(char *str, const char *format, ...);
int (*snprintf)(char *str, size_t size, const char *format, ...);
int (*vsprintf)(char *s, const char *format, va_list arg);
int (*sscanf)(const char *str, const char *format, ...);
char *(*strchr)(const char *s, int c);
char *(*strrchr)(const char *s, int c);
char *(*strstr)(char *str1, char *str2);
int (*atoi)(const char *str);
double (*atof)(const char *str);
double (*sin)(double x);
float (*sinf)(float x);
double (*cos)(double x);
float (*cosf)(float x);
double (*tan)(double x);
float (*tanf)(float x);
double (*atan)(double x);
double (*atan2)(double x, double y);
double (*sqrt)(double vec);
float (*sqrtf)(float vec);
double (*pow)(double x, double y);
int (*abs)(int n);

void *sys_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset) {
	return (void *)syscall(477, addr, len, prot, flags, fd, offset);
}

void *sys_munmap(void *addr, size_t len) {
	return (void *)syscall(479, addr, len);
}

void initSyscall() {
	ptr_syscall = (void*)((uint64_t)getpid + 0x0A);
}

void initImports(dlsym_t* dlsym) {
	int libKernel = 0x2001;


	// Kernel
	dlsym(libKernel, "sceKernelLoadStartModule", &sceKernelLoadStartModule);
	dlsym(libKernel, "sceKernelDebugOutText", &sceKernelDebugOutText);
	dlsym(libKernel, "sceKernelMprotect", &sceKernelMprotect);
	dlsym(libKernel, "sceKernelSleep", &sceKernelSleep);
	dlsym(libKernel, "sceKernelUsleep", &sceKernelUsleep);
	dlsym(libKernel, "sceKernelSendNotificationRequest", &sceKernelSendNotificationRequest);
	dlsym(libKernel, "getpid", &getpid);
	dlsym(libKernel, "sceKernelJitCreateSharedMemory", &sceKernelJitCreateSharedMemory);
	dlsym(libKernel, "sceKernelJitCreateAliasOfSharedMemory", &sceKernelJitCreateAliasOfSharedMemory);
	dlsym(libKernel, "sceKernelJitMapSharedMemory", &sceKernelJitMapSharedMemory);
	dlsym(libKernel, "sceKernelWrite", &sceKernelWrite);
	dlsym(libKernel, "sceKernelRead", &sceKernelRead);
	dlsym(libKernel, "sysctlbyname", &sysctlbyname);


	// Threads
	dlsym(libKernel, "scePthreadCreate", &scePthreadCreate);
	dlsym(libKernel, "scePthreadExit", &scePthreadExit);
	dlsym(libKernel, "scePthreadDetach", &scePthreadDetach);
	dlsym(libKernel, "scePthreadJoin", &scePthreadJoin);
	dlsym(libKernel, "scePthreadGetprio", &scePthreadGetprio);


	// Network
	int libNet = sceKernelLoadStartModule("libSceNet.sprx", 0, 0, 0, 0, 0);
	dlsym(libNet, "sceNetSocket", &sceNetSocket);
	dlsym(libNet, "sceNetSocketClose", &sceNetSocketClose);
	dlsym(libNet, "sceNetConnect", &sceNetConnect);
	dlsym(libNet, "sceNetSend", &sceNetSend);
	dlsym(libNet, "sceNetSetsockopt", &sceNetSetsockopt);

	// Message dialogs
	int libDialog = sceKernelLoadStartModule("libSceCommonDialog.sprx", 0, 0, 0, 0, 0);
	dlsym(libDialog, "sceCommonDialogInitialize", &sceCommonDialogInitialize);

	int libMsgDialog = sceKernelLoadStartModule("libSceMsgDialog.sprx", 0, 0, 0, 0, 0);
	dlsym(libMsgDialog, "sceMsgDialogInitialize", &sceMsgDialogInitialize);
	dlsym(libMsgDialog, "sceMsgDialogOpen", &sceMsgDialogOpen);
	dlsym(libMsgDialog, "sceMsgDialogUpdateStatus", &sceMsgDialogUpdateStatus);
	dlsym(libMsgDialog, "sceMsgDialogTerminate", &sceMsgDialogTerminate);

	//ime dialogs
	int libImeDialog = sceKernelLoadStartModule("libSceImeDialog.sprx", 0, 0, 0, 0, 0);
	dlsym(libImeDialog, "sceImeDialogGetResult", &sceImeDialogGetResult);
	dlsym(libImeDialog, "sceImeDialogInit", &sceImeDialogInit);
	dlsym(libImeDialog, "sceImeDialogGetStatus", &sceImeDialogGetStatus);
	dlsym(libImeDialog, "sceImeDialogTerm", &sceImeDialogTerm);

	//web dialogs
	int libBrowserDialog = sceKernelLoadStartModule("libSceWebBrowserDialog.sprx", 0, 0, 0, 0, 0);
	dlsym(libBrowserDialog, "sceWebBrowserDialogInitialize", &sceWebBrowserDialogInitialize);
	dlsym(libBrowserDialog, "sceWebBrowserDialogTerminate", &sceWebBrowserDialogTerminate);
	dlsym(libBrowserDialog, "sceWebBrowserDialogOpen", &sceWebBrowserDialogOpen);
	dlsym(libBrowserDialog, "sceWebBrowserDialogUpdateStatus", &sceWebBrowserDialogUpdateStatus);
	dlsym(libBrowserDialog, "sceWebBrowserDialogSetCookie", &sceWebBrowserDialogSetCookie);

	//user service
	int libUserService = sceKernelLoadStartModule("libSceUserService.sprx", 0, 0, 0, 0, 0);
	dlsym(libUserService, "sceUserServiceGetInitialUser", &sceUserServiceGetInitialUser);
	dlsym(libUserService, "sceUserServiceGetUserName", &sceUserServiceGetUserName);
	dlsym(libUserService, "sceUserServiceGetLoginUserIdList", &sceUserServiceGetLoginUserIdList);

	//gamepad
	int libScePad = sceKernelLoadStartModule("libScePad.sprx", 0, 0, 0, 0, 0);
	dlsym(libScePad, "scePadOpen", &scePadOpen);
	dlsym(libScePad, "scePadGetHandle", &scePadGetHandle);
	dlsym(libScePad, "scePadSetLightBar", &scePadSetLightBar);
	dlsym(libScePad, "scePadReadState", &scePadReadState);

	//libc
	int libC = sceKernelLoadStartModule("libSceLibcInternal.sprx", 0, 0, 0, 0, 0);
	dlsym(libC, "bzero", &bzero);
	dlsym(libC, "malloc", &malloc);
	dlsym(libC, "free", &free);
	dlsym(libC, "calloc", &calloc);
	dlsym(libC, "realloc", &realloc);
	dlsym(libC, "memset", &memset);
	dlsym(libC, "memcpy", &memcpy);
	dlsym(libC, "memcmp", &memcmp);
	dlsym(libC, "memmove", &memmove);
	dlsym(libC, "mbstowcs", &mbstowcs);
	dlsym(libC, "wcstombs", &wcstombs);
	dlsym(libC, "strcpy", &strcpy);
	dlsym(libC, "strncpy", &strncpy);
	dlsym(libC, "strcat", &strcat);
	dlsym(libC, "strncat", &strncat);
	dlsym(libC, "strlen", &strlen);
	dlsym(libC, "strcmp", &strcmp);
	dlsym(libC, "strncmp", &strncmp);
	dlsym(libC, "sprintf", &sprintf);
	dlsym(libC, "snprintf", &snprintf);
	dlsym(libC, "vsprintf", &vsprintf);
	dlsym(libC, "sscanf", &sscanf);
	dlsym(libC, "strchr", &strchr);
	dlsym(libC, "strrchr", &strrchr);
	dlsym(libC, "strstr", &strstr);
	dlsym(libC, "atoi", &atoi);
	dlsym(libC, "atof", &atof);
	dlsym(libC, "sin", &sin);
	dlsym(libC, "sinf", &sinf);
	dlsym(libC, "cos", &cos);
	dlsym(libC, "cosf", &cosf);
	dlsym(libC, "tan", &tan);
	dlsym(libC, "tanf", &tanf);
	dlsym(libC, "atan", &atan);
	dlsym(libC, "atan2", &atan2);
	dlsym(libC, "sqrt", &sqrt);
	dlsym(libC, "sqrtf", &sqrtf);
	dlsym(libC, "pow", &pow);
	dlsym(libC, "abs", &abs);
}
