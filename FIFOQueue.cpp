// FIFOQueue.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "FIFOQueue.h"
#include "MemPool.h"

CFIFOQueue::CFIFOQueue( size_t szEveryBufSize, size_t szCount )
	:m_szBufSize(szEveryBufSize), m_szCount(szCount)
{
	m_pPool = new CMemPool(szEveryBufSize, szCount);
}

CFIFOQueue::~CFIFOQueue()
{
	m_pPool->FreeAll();
	delete m_pPool;
	m_pPool = NULL;
}

bool CFIFOQueue::AllocQueue()
{
	return m_pPool->AllocPool();
}

bool CFIFOQueue::FreeQueue()
{
	return m_pPool->FreeAll();
}

int CFIFOQueue::PutData( char *pSrcData, size_t szLen, size_t szWaitSecs /*= 0*/ )
{
	return m_pPool->PutData(pSrcData, szLen, szWaitSecs);
}

int CFIFOQueue::GetData( char *pDstData, size_t szRecvLen, size_t szWaitSecs /*= 0*/ )
{
	return m_pPool->GetData(pDstData, szRecvLen, szWaitSecs);
}

