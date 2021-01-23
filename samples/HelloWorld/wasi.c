#include <stdio.h>

int *__cxa_thread_atexit(void (*func)(), void *obj,
                                   void *dso_symbol)
{
	printf("__cxa_thread_atexit\n");
	return 0;
}

void (*pthread_create(int sig, int i, int j, void (*func)(int)))(int)
{
	printf("pthread_create\n");
	return 0;
}

void (*signal(int sig, void (*func)(int)))(int)
{
	printf("signal %d\n", sig);
	return 0;
}
void (*gai_strerror(int sig ))(int)
{
	printf("gai_strerror\n");
	return 0;
}
void (*flock(int sig, int i ))(int)
{
	printf("flock\n");
	return 0;
}
int __syscall220(int i, int j, int k)
{
	printf("__syscall220\n");
	return 0;
}
int __syscall272(int i, int j, int k, int i4, int i5, int i6, int i7)
{
	printf("__sys_fadvise64_64\n");
	return 0;
}
int __syscall340(int i, int j, int k, int i4)
{
	printf("__sys_prlimit64\n");
	return 0;
}
int __syscall191(int i, int j)
{
	printf("__sys_ugetrlimit\n");
	return 0;
}
int __syscall77(int i, int j)
{
	printf("__sys_getrusage\n");
	return 0;
}
int __syscall219(int i, int j, int k)
{
	printf("__sys_madvise1\n");
	return 0;
}
int __syscall150(int i, int j)
{
	printf("__sys_mlock\n");
	return 0;
}
int __syscall125(int i, int j, int k)
{
	printf("__sys_mprotect\n");
	return 0;
}
int __syscall151(int i, int j)
{
	printf("__sys_munlock\n");
	return 0;
}
int __syscall197(int i, int j)
{
	printf("__sys_fstat64\n");
	return 0;
}
int __syscall195(int i, int j)
{
	printf("__sys_stat64\n");
	return 0;
}
int __syscall196(int i, int j)
{
	printf("__sys_lstat64\n");
	return 0;
}
int __syscall194(int i, int j, int k, int i4)
{
	printf("__sys_ftruncate64\n");
	return 0;
}
int __syscall183(int i, int j)
{
	printf("__sys_getcwd\n");
	return 0;
}
int __syscall20()
{
	printf("__sys_getpid\n");
	return 0;
}
int __syscall85(int i, int j, int k)
{
	printf("__sys_readlink\n");
	return 0;
}
void __syscall10(int i, int j4)
{
	printf("__sys_unlink\n");
}
void emscripten_thread_sleep(double j4)
{
	printf("emscripten_thread_sleep\n");
}
void emscripten_notify_memory_growth(int i)
{
	printf("emscripten_notify_memory_growth\n");
}
int __syscall102(int i, int j4)
{
	printf("__sys_socketcall\n");
	return 0;
}
