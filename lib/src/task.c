// ---------------------------------------------------------------------------
//                         Copyright Joe Drago 2018.
//         Distributed under the Boost Software License, Version 1.0.
//            (See accompanying file LICENSE_1_0.txt or copy at
//                  http://www.boost.org/LICENSE_1_0.txt)
// ---------------------------------------------------------------------------

#include "colorist/task.h"

static void nativeTaskStart(clTask * task);
static void nativeTaskJoin(clTask * task);

clTask * clTaskCreate(clTaskFunc func, void * userData)
{
    clTask * task = clAllocate(clTask);
    task->func = func;
    task->nativeData = NULL;
    task->userData = userData;
    task->joined = clFalse;
    nativeTaskStart(task);
    return task;
}

void clTaskJoin(clTask * task)
{
    if (!task->joined) {
        nativeTaskJoin(task);
        task->joined = clTrue;
    }
}

void clTaskDestroy(clTask * task)
{
    clTaskJoin(task);
    COLORIST_ASSERT(task->joined);
    COLORIST_ASSERT(task->nativeData == NULL);
    free(task);
}

#ifdef _WIN32

#include <windows.h>

int clTaskLimit()
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    int numCPU = sysinfo.dwNumberOfProcessors;
    return numCPU;
}

typedef struct clNativeTask
{
    HANDLE hThread;
} clNativeTask;

static DWORD WINAPI taskThreadProc(LPVOID lpParameter)
{
    clTask * task = (clTask *)lpParameter;
    task->func(task->userData);
    return 0;
}

static void nativeTaskStart(clTask * task)
{
    DWORD threadId;
    clNativeTask * nativeTask = clAllocate(clNativeTask);
    task->nativeData = nativeTask;
    nativeTask->hThread = CreateThread(NULL, 0, taskThreadProc, task, 0, &threadId);
}

static void nativeTaskJoin(clTask * task)
{
    clNativeTask * nativeTask = (clNativeTask *)task->nativeData;
    WaitForSingleObject(nativeTask->hThread, INFINITE);
    CloseHandle(nativeTask->hThread);
    free(task->nativeData);
    task->nativeData = NULL;
}

#else /* ifdef _WIN32 */
#error implement pthreads
#endif /* ifdef _WIN32 */