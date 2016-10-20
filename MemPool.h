#pragma once
#include "boost\pool\pool.hpp"
#include "boost\thread\condition_variable.hpp"
#include "boost\thread\recursive_mutex.hpp"
#include <vector>

using namespace std;
class CMemPool
{
public:
	CMemPool(size_t szEveryBufSize, size_t szCount);
	~CMemPool(void);
public:
	bool AllocPool();
	bool FreeAll();
	int PutData(char *pData, size_t szLen, size_t szWaitSecs = 0);
	int GetData(char *pData, size_t szRecvLen, size_t szWaitSecs = 0);
private:
	boost::pool<> m_Pool;
	size_t m_szCount;
	size_t m_szGetIndex;
	size_t m_szPutIndex;
	size_t m_szMaxBufferSize;
	vector<const char*> m_lstPtr;
	size_t m_szDataNum;
	boost::recursive_mutex lock_mutex; 
	boost::condition_variable_any get_Cond;
	boost::condition_variable_any put_Cond;
};
