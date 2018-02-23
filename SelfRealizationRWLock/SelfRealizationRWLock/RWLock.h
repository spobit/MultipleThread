#pragma once
#include <Windows.h>
#include <assert.h>


class CRWLock
{
private:
	int m_iIsReading;			//正在读计数
	int m_iPrepareWritting;		//准备写计数
	
	HANDLE m_hGeneralEvent;		//通用自动事件,内部最好不要有阻塞
	HANDLE m_hReadEvent;		//读手动事件
	HANDLE m_hWriteEvent;		//写自动事件

public:
	CRWLock();
	~CRWLock();
	CRWLock(const CRWLock&) { assert(0); }
	CRWLock& operator=(const CRWLock&) { assert(0); }

	void LockRead();
	void UnlockRead();
	void LockWrite();
	void UnlockWrite();
};

