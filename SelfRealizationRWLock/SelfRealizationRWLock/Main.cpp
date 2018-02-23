#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include "RWLock.h"


int g_i = 0;
CRWLock g_oRWLock;

void ReadThreadCallback();
void WriteThreadCallback();

int main()
{
	HANDLE hThreadArr[10]{};
	hThreadArr[0] = (HANDLE)::_beginthread((_beginthread_proc_type)ReadThreadCallback, 0, 0);
	hThreadArr[1] = (HANDLE)::_beginthread((_beginthread_proc_type)ReadThreadCallback, 0, 0);
	hThreadArr[2] = (HANDLE)::_beginthread((_beginthread_proc_type)WriteThreadCallback, 0, 0);
	hThreadArr[3] = (HANDLE)::_beginthread((_beginthread_proc_type)ReadThreadCallback, 0, 0);
	hThreadArr[4] = (HANDLE)::_beginthread((_beginthread_proc_type)ReadThreadCallback, 0, 0);
	hThreadArr[5] = (HANDLE)::_beginthread((_beginthread_proc_type)WriteThreadCallback, 0, 0);
	hThreadArr[6] = (HANDLE)::_beginthread((_beginthread_proc_type)ReadThreadCallback, 0, 0);
	hThreadArr[7] = (HANDLE)::_beginthread((_beginthread_proc_type)ReadThreadCallback, 0, 0);
	hThreadArr[8] = (HANDLE)::_beginthread((_beginthread_proc_type)WriteThreadCallback, 0, 0);
	hThreadArr[9] = (HANDLE)::_beginthread((_beginthread_proc_type)ReadThreadCallback, 0, 0);
	::WaitForMultipleObjects(10, hThreadArr, 1, INFINITE);

	printf("%08x\n", g_i);

	return 0;
}

void ReadThreadCallback()
{
	for (int i = 0; i < 0x1000; i++)
	{
		g_oRWLock.LockRead();
		int iTmp = g_i;
		DWORD dwId = GetCurrentThreadId();
		printf("Read :%08x-%08x\n", dwId, g_i);
		g_oRWLock.UnlockRead();
	}
}

void WriteThreadCallback()
{
	for (int i = 0; i < 0x1000; i++)
	{
		g_oRWLock.LockWrite();
		g_i++;
		DWORD dwId = GetCurrentThreadId();
		printf("Write:%08x-%08x\n", dwId, g_i);
		g_oRWLock.UnlockWrite();
	}
}
