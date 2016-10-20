#pragma once

/*
	名称:FIFOQueue.h
	库介绍：此库为先进先出缓冲队列库，可用于异步读写数据
	版本：1.0
	作者:zhangfang 20140220
*/

#ifdef FIFOQUEUE_CPP
#define DllImportOrExport	__declspec(dllexport)
#else
#define DllImportOrExport	__declspec(dllimport)
#endif

const int OUT_OF_RANGE		=	-1;
const int WAIT_TIME_OUT		=	-255;
const int SUCCESS			 = 0;
const int NO_MEMORY_IN_QUEUE = -256;

class CMemPool;

/*构造实例之后 ，需调用AllocQueue函数进行真正进行队列分配*/
class  DllImportOrExport CFIFOQueue
{
public:
	/*szEveryBufSize: 每一块大小
	  szCount:分配多少块
	  注意：申请过大可能会导致崩溃
	*/
	CFIFOQueue(size_t szEveryBufSize, size_t szCount);
	~CFIFOQueue();
public:
	/*必须调用此函数进行分配，否则此列表无法使用*/
	bool AllocQueue();
	bool FreeQueue();
	/*放入数据，如果队列已满则等待，如果szWaitSecs不为0，则等待此秒数后失败返回*/
	int PutData(char *pSrcData, size_t szLen, size_t szWaitSecs = 0);
	/*获取数据，如果队列为空则等待*/
	int GetData(char *pDstData, size_t szRecvLen, size_t szWaitSecs = 0);
protected:
private:
	CMemPool *m_pPool;
	size_t m_szBufSize;
	size_t m_szCount;
};