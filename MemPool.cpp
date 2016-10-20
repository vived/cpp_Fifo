#include "StdAfx.h"
#include "MemPool.h"
#include "FIFOQueue.h"


CMemPool::~CMemPool(void)
{
}

CMemPool::CMemPool( size_t szEveryBufSize, size_t szCount )
	:m_Pool(szEveryBufSize), m_szMaxBufferSize(szEveryBufSize), m_szCount(szCount)
{
	m_lstPtr.clear();
	m_szGetIndex = 0;
	m_szPutIndex = 0;
	m_szDataNum = 0;
}

bool CMemPool::AllocPool()
{
	if (m_lstPtr.size() != 0)
	{
		return false;
	}
	try
	{
		m_Pool.set_max_size(ULONG_MAX);
		for (size_t i = 0; i < m_szCount; i++)
		{
			char *pAlloc = (char*)m_Pool.malloc();
			if (NULL == pAlloc)
			{
				FreeAll();
				return false;
			}
			memset(pAlloc, 0, m_szMaxBufferSize);
			m_lstPtr.push_back(pAlloc);
		}
	}
	catch (...)
	{
		FreeAll();
		return false;
	}
	return true;
}

bool CMemPool::FreeAll()
{
	boost::recursive_mutex::scoped_lock lo(lock_mutex);
	for (size_t szCount = 0; szCount < m_lstPtr.size(); ++szCount)
	{
		m_Pool.free((void*)m_lstPtr[szCount]);
	}
	m_lstPtr.clear();
	return true;
}

int CMemPool::PutData( char *pData, size_t szLen, size_t szWaitSecs /*= 0*/ )
{
	if (0 == m_lstPtr.size())
	{
		return NO_MEMORY_IN_QUEUE;
	}
	if (szLen > m_szMaxBufferSize)
	{
		return OUT_OF_RANGE;
	}
	boost::recursive_mutex::scoped_lock lo(lock_mutex);

	if (m_lstPtr.size() == m_szDataNum)
	{
		if (0 == szWaitSecs)
		{
			put_Cond.wait(lock_mutex);			
		}
		else
		{
			if (!put_Cond.timed_wait(lock_mutex, boost::get_system_time() + boost::posix_time::seconds(szWaitSecs)))
			{
				return WAIT_TIME_OUT;
			}
		}
	}

	m_szDataNum++;			
	memcpy_s((void*)m_lstPtr[m_szPutIndex], m_szMaxBufferSize, pData, szLen);
	m_szPutIndex = (++m_szPutIndex)%(m_lstPtr.size());	
	get_Cond.notify_one();

	return ERROR_SUCCESS;
}

int CMemPool::GetData( char *pData, size_t szRecvLen, size_t szWaitSecs /*= 0*/ )
{
	if (0 == m_lstPtr.size())
	{
		return NO_MEMORY_IN_QUEUE;
	}
	if (szRecvLen > m_szMaxBufferSize)
	{
		return OUT_OF_RANGE;
	}
	boost::recursive_mutex::scoped_lock lo(lock_mutex);
	if (0 == m_szDataNum)
	{
		if (0 == szWaitSecs)
		{
			get_Cond.wait(lock_mutex);			
		}
		else
		{
			if (!get_Cond.timed_wait(lock_mutex, boost::get_system_time() + boost::posix_time::seconds(szWaitSecs)))
			{
				return WAIT_TIME_OUT;
			}			
		}
	}
	m_szDataNum--;
	memcpy_s(pData, szRecvLen, (void*)m_lstPtr[m_szGetIndex], szRecvLen);
	memset((void*)m_lstPtr[m_szGetIndex], 0, m_szMaxBufferSize);
	m_szGetIndex = ++m_szGetIndex%(m_lstPtr.size() );		
	put_Cond.notify_one();
	return ERROR_SUCCESS;
}
