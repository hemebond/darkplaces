// #include <SDL.h>
// #include <SDL_thread.h>
#include <SDL3/SDL.h>
#include "quakedef.h"
#include "thread.h"

int Thread_Init(void)
{
#ifdef THREADDISABLE
	Con_Printf("Threading disabled in this build\n");
#endif
	return 0;
}

void Thread_Shutdown(void)
{
}

qbool Thread_HasThreads(void)
{
#ifdef THREADDISABLE
	return false;
#else
	return true;
#endif
}

void *_Thread_CreateMutex(const char *filename, int fileline)
{
	void *mutex = SDL_CreateMutex();
#ifdef THREADDEBUG
	Sys_Printf("%p mutex create %s:%i\n" , mutex, filename, fileline);
#endif
	return mutex;
}

void _Thread_DestroyMutex(void *mutex, const char *filename, int fileline)
{
#ifdef THREADDEBUG
	Sys_Printf("%p mutex destroy %s:%i\n", mutex, filename, fileline);
#endif
	SDL_DestroyMutex((SDL_Mutex *)mutex);
}

void _Thread_LockMutex(void *mutex, const char *filename, int fileline)
{
#ifdef THREADDEBUG
	Sys_Printf("%p mutex lock %s:%i\n"   , mutex, filename, fileline);
#endif
	SDL_LockMutex((SDL_Mutex *)mutex);
}

void _Thread_UnlockMutex(void *mutex, const char *filename, int fileline)
{
#ifdef THREADDEBUG
	Sys_Printf("%p mutex unlock %s:%i\n" , mutex, filename, fileline);
#endif
	SDL_UnlockMutex((SDL_Mutex *)mutex);
}

void *_Thread_CreateCond(const char *filename, int fileline)
{
	void *cond = (void *)SDL_CreateCondition();
#ifdef THREADDEBUG
	Sys_Printf("%p cond create %s:%i\n"   , cond, filename, fileline);
#endif
	return cond;
}

void _Thread_DestroyCond(void *cond, const char *filename, int fileline)
{
#ifdef THREADDEBUG
	Sys_Printf("%p cond destroy %s:%i\n"   , cond, filename, fileline);
#endif
	SDL_DestroyCondition((SDL_Condition *)cond);
}

void _Thread_CondSignal(void *cond, const char *filename, int fileline)
{
#ifdef THREADDEBUG
	Sys_Printf("%p cond signal %s:%i\n"   , cond, filename, fileline);
#endif
	SDL_SignalCondition((SDL_Condition *)cond);
}

void _Thread_CondBroadcast(void *cond, const char *filename, int fileline)
{
#ifdef THREADDEBUG
	Sys_Printf("%p cond broadcast %s:%i\n"   , cond, filename, fileline);
#endif
	SDL_BroadcastCondition((SDL_Condition *)cond);
}

void _Thread_CondWait(void *cond, void *mutex, const char *filename, int fileline)
{
#ifdef THREADDEBUG
	Sys_Printf("%p cond wait %s:%i\n"   , cond, filename, fileline);
#endif
	SDL_WaitCondition((SDL_Condition *)cond, (SDL_Mutex *)mutex);
}

void *_Thread_CreateThread(int (*fn)(void *), void *data, const char *filename, int fileline)
{
	void *thread = (void *)SDL_CreateThread(fn, filename, data);
#ifdef THREADDEBUG
	Sys_Printf("%p thread create %s:%i\n"   , thread, filename, fileline);
#endif
	return thread;
}

int _Thread_WaitThread(void *thread, int retval, const char *filename, int fileline)
{
	int status = retval;
#ifdef THREADDEBUG
	Sys_Printf("%p thread wait %s:%i\n"   , thread, filename, fileline);
#endif
	SDL_WaitThread((SDL_Thread *)thread, &status);
	return status;
}

// standard barrier implementation using conds and mutexes
// see: http://www.howforge.com/implementing-barrier-in-pthreads
typedef struct {
	unsigned int needed;
	unsigned int called;
	void *mutex;
	void *cond;
} barrier_t;

void *_Thread_CreateBarrier(unsigned int count, const char *filename, int fileline)
{
	volatile barrier_t *b = (volatile barrier_t *) Z_Malloc(sizeof(barrier_t));
#ifdef THREADDEBUG
	Sys_Printf("%p barrier create(%d) %s:%i\n", b, count, filename, fileline);
#endif
	b->needed = count;
	b->called = 0;
	b->mutex = Thread_CreateMutex();
	b->cond = Thread_CreateCond();
	return (void *) b;
}

void _Thread_DestroyBarrier(void *barrier, const char *filename, int fileline)
{
	volatile barrier_t *b = (volatile barrier_t *) barrier;
#ifdef THREADDEBUG
	Sys_Printf("%p barrier destroy %s:%i\n", b, filename, fileline);
#endif
	Thread_DestroyMutex(b->mutex);
	Thread_DestroyCond(b->cond);
}

void _Thread_WaitBarrier(void *barrier, const char *filename, int fileline)
{
	volatile barrier_t *b = (volatile barrier_t *) barrier;
#ifdef THREADDEBUG
	Sys_Printf("%p barrier wait %s:%i\n", b, filename, fileline);
#endif
	Thread_LockMutex(b->mutex);
	b->called++;
	if (b->called == b->needed) {
		b->called = 0;
		Thread_CondBroadcast(b->cond);
	} else {
		do {
			Thread_CondWait(b->cond, b->mutex);
		} while(b->called);
	}
	Thread_UnlockMutex(b->mutex);
}

int _Thread_AtomicGet(Thread_Atomic *a, const char *filename, int fileline)
{
#ifdef THREADDEBUG
	Sys_Printf("%p atomic get at %s:%i\n", a, filename, fileline);
#endif
	return SDL_GetAtomicInt((SDL_AtomicInt *)a);
}

int _Thread_AtomicSet(Thread_Atomic *a, int v, const char *filename, int fileline)
{
#ifdef THREADDEBUG
	Sys_Printf("%p atomic set %v at %s:%i\n", a, v, filename, fileline);
#endif
	return SDL_SetAtomicInt((SDL_AtomicInt *)a, v);
}

int _Thread_AtomicAdd(Thread_Atomic *a, int v, const char *filename, int fileline)
{
#ifdef THREADDEBUG
	Sys_Printf("%p atomic add %v at %s:%i\n", a, v, filename, fileline);
#endif
	return SDL_AddAtomicInt((SDL_AtomicInt *)a, v);
}

void _Thread_AtomicIncRef(Thread_Atomic *a, const char *filename, int fileline)
{
#ifdef THREADDEBUG
	Sys_Printf("%p atomic incref %s:%i\n", a, filename, fileline);
#endif
	SDL_AtomicIncRef((SDL_AtomicInt *)a);
}

qbool _Thread_AtomicDecRef(Thread_Atomic *a, const char *filename, int fileline)
{
#ifdef THREADDEBUG
	Sys_Printf("%p atomic decref %s:%i\n", a, filename, fileline);
#endif
	return SDL_AtomicDecRef((SDL_AtomicInt *)a) != false;
}

qbool _Thread_AtomicTryLock(Thread_SpinLock *lock, const char *filename, int fileline)
{
#ifdef THREADDEBUG
	Sys_Printf("%p atomic try lock %s:%i\n", lock, filename, fileline);
#endif
	return SDL_TryLockSpinlock(lock) != false;
}

void _Thread_AtomicLock(Thread_SpinLock *lock, const char *filename, int fileline)
{
#ifdef THREADDEBUG
	Sys_Printf("%p atomic lock %s:%i\n", lock, filename, fileline);
#endif
	SDL_LockSpinlock(lock);
}

void _Thread_AtomicUnlock(Thread_SpinLock *lock, const char *filename, int fileline)
{
#ifdef THREADDEBUG
	Sys_Printf("%p atomic unlock %s:%i\n", lock, filename, fileline);
#endif
	SDL_UnlockSpinlock(lock);
}
