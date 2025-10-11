// Minimal compatibility shim to expose C11-like thrd_t/mtx_t/cnd_t APIs on Windows
// so the chessapi code can compile without changing its threading calls.
// This is intentionally small and only implements the functions used in the
// project (create, detach/join not required here, mutex init/lock/unlock,
// condition variable wait/signal).

#pragma once

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#include <stdbool.h>

// Types
typedef HANDLE thrd_t;
typedef CRITICAL_SECTION mtx_t;
typedef CONDITION_VARIABLE cnd_t;

// mtx flags expected by code using C11 threads.h
#ifndef mtx_plain
#define mtx_plain 0
#endif
#ifndef mtx_timed
#define mtx_timed 1
#endif
#ifndef mtx_recursive
#define mtx_recursive 2
#endif

// Thread creation (wrap _beginthreadex)
static inline int thrd_create(thrd_t *thr, int (*func)(void*), void *arg) {
    (void)thr; // we don't use thr value elsewhere in this shim
    uintptr_t handle = _beginthreadex(NULL, 0, (unsigned (__stdcall *)(void *))func, arg, 0, NULL);
    if (handle == 0) return -1;
    *thr = (HANDLE)handle;
    return 0;
}

// Mutex
static inline int mtx_init(mtx_t *mtx, int type) {
    (void) type;
    InitializeCriticalSection(mtx);
    return 0;
}

static inline void mtx_destroy(mtx_t *mtx) {
    DeleteCriticalSection(mtx);
}

static inline int mtx_lock(mtx_t *mtx) {
    EnterCriticalSection(mtx);
    return 0;
}

static inline int mtx_unlock(mtx_t *mtx) {
    LeaveCriticalSection(mtx);
    return 0;
}

// Condition variable
static inline int cnd_init(cnd_t *cv) {
    InitializeConditionVariable(cv);
    return 0;
}

static inline int cnd_signal(cnd_t *cv) {
    WakeConditionVariable(cv);
    return 0;
}

static inline int cnd_broadcast(cnd_t *cv) {
    WakeAllConditionVariable(cv);
    return 0;
}

static inline int cnd_wait(cnd_t *cv, mtx_t *mtx) {
    // SleepConditionVariableCS returns nonzero on success
    BOOL ok = SleepConditionVariableCS(cv, mtx, INFINITE);
    return ok ? 0 : -1;
}

#endif // _WIN32
