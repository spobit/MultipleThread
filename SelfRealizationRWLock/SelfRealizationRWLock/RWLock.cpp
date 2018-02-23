#include "RWLock.h"



CRWLock::CRWLock()
{
	m_iIsReading = 0;
	m_iPrepareWritting = 0;

	m_hGeneralEvent = ::CreateEvent(0, 0, 1, 0);
	assert(0 != m_hGeneralEvent);

	m_hReadEvent = ::CreateEvent(0, 1, 1, 0);
	assert(0 != m_hReadEvent);

	m_hWriteEvent = ::CreateEvent(0, 0, 1, 0);
	assert(0 != m_hWriteEvent);
}


CRWLock::~CRWLock()
{
	::CloseHandle(m_hGeneralEvent);
	::CloseHandle(m_hReadEvent);
	::CloseHandle(m_hWriteEvent);
}

void CRWLock::LockRead()
{
	HANDLE hArr[2] = { m_hGeneralEvent, m_hReadEvent };
	DWORD dwRet = ::WaitForMultipleObjects(2, hArr, 1, INFINITE);
	assert(WAIT_OBJECT_0 == dwRet);
	{
		m_iIsReading++;
		::ResetEvent(m_hWriteEvent);
	}
	::SetEvent(m_hGeneralEvent);
}

void CRWLock::UnlockRead()
{
	DWORD dwRet = ::WaitForSingleObject(m_hGeneralEvent, INFINITE);
	assert(WAIT_OBJECT_0 == dwRet);
	{
		if (--m_iIsReading == 0)
			::SetEvent(m_hWriteEvent);
	}
	::SetEvent(m_hGeneralEvent);
}

void CRWLock::LockWrite()
{
	DWORD dwRet = ::WaitForSingleObject(m_hGeneralEvent, INFINITE);
	assert(WAIT_OBJECT_0 == dwRet);
	{
		m_iPrepareWritting++;
		::ResetEvent(m_hReadEvent);

		//dwRet = ::WaitForSingleObject(m_hWriteEvent, INFINITE);	//这里会死锁
		//assert(WAIT_OBJECT_0 == dwRet);
	}
	::SetEvent(m_hGeneralEvent);

	dwRet = ::WaitForSingleObject(m_hWriteEvent, INFINITE);
	assert(WAIT_OBJECT_0 == dwRet);
}

void CRWLock::UnlockWrite()
{
	DWORD dwRet = ::WaitForSingleObject(m_hGeneralEvent, INFINITE);
	assert(WAIT_OBJECT_0 == dwRet);
	{
		::SetEvent(m_hWriteEvent);
		if (--m_iPrepareWritting == 0)
			::SetEvent(m_hReadEvent);
	}
	::SetEvent(m_hGeneralEvent);
}
