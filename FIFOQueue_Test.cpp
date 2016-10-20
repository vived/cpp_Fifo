#include "stdafx.h"


//#include "fifoqueue.h"
#include "../FIFOQueue/FIFOQueue.h"

 #include <windows.h>
 #include <process.h>

#if 0

static CFIFOQueue qu(10,2);
static CFIFOQueue que(10,3);

//没有分配，直接使用,期望值NO_MEMORY_IN_QUEUE
TEST(FIFOQueue,PutData1)
{
	char *pSrcData = "123";
	ASSERT_EQ(NO_MEMORY_IN_QUEUE,qu.PutData(pSrcData,strlen(pSrcData)));
}
//分配缓冲区
TEST(FIFOQueue, Allocate)
{
	//EXPECT_EQ(true, qu.AllocQueue());
	ASSERT_TRUE(qu.AllocQueue());
}
//不向队列中put数据，直接get数据,期望值WAIT_TIME_OUT
 TEST(FIFOQueue,GetData1)//no values
 {
	 char pDstData[32] = {0};
 	ASSERT_EQ(WAIT_TIME_OUT,qu.GetData(pDstData,3, 1));
 }
//正常put数据，期望值SUCCESS
TEST(FIFOQueue, PutData2)
{
	char *pSrcData = "123";
	ASSERT_EQ(SUCCESS,qu.PutData(pSrcData,strlen(pSrcData)));
}
//put的数据超过数据块的大小,期望值OUT_OF_RANGE
TEST(FIFOQueue,PutData3)
{
	char *pSrcData = "98765432101";
	ASSERT_EQ(OUT_OF_RANGE,qu.PutData(pSrcData,strlen(pSrcData)));
}
//put数据，超过分配的块数，期望值WAIT_TIME_OUT
TEST(FIFOQueue,PutData4)
{
	char *pSrcData = "9876";
 	qu.PutData(pSrcData,strlen(pSrcData));

	pSrcData = "456";
	ASSERT_EQ(WAIT_TIME_OUT,qu.PutData(pSrcData,strlen(pSrcData),1));//超出分配的块数
}
//put数据，超过分配的块数，等待2s，在等待期间get数据，再进行put数据，期望值SUCCESS
unsigned __stdcall GetDataThread_PutData5(void* pArguments)
{
 	char pDstData[32] = {0};
 	Sleep(2000);
 	qu.GetData(pDstData,4);
 
 	return 0;

}
TEST(FIFOQueue,PutData5)
{
 	HANDLE hThread;
 	unsigned threadID;

	hThread = (HANDLE)_beginthreadex( NULL, 0, &GetDataThread_PutData5, NULL, 0, &threadID );

	char *pSrcData = "456";
	ASSERT_EQ(SUCCESS,qu.PutData(pSrcData, strlen(pSrcData), 3));

	CloseHandle( hThread );
}

//put数据，超过分配的块数，等待2s，在等待期间get数据，再进行put数据
unsigned __stdcall GetDataThread_PutData6(void* pArguments)
{
	char pDstData[32] = {0};
	Sleep(2000);
	qu.GetData(pDstData,4);


	return 0;

}
TEST(FIFOQueue,PutData6)
{
	HANDLE hThread;
	unsigned threadID;

	hThread = (HANDLE)_beginthreadex( NULL, 0, &GetDataThread_PutData6, NULL, 0, &threadID );

	char *pSrcData = "456";
	ASSERT_EQ(WAIT_TIME_OUT,qu.PutData(pSrcData, strlen(pSrcData), 1));//等待1s后返回，期望值是WAIT_TIME_OUT

	CloseHandle( hThread );
}
//队列中有数据，正常get数据，期望值SUCCESS
 TEST(FIFOQueue,GetData2)
 {
 	char pDstData[32] = {0};
 	ASSERT_EQ(SUCCESS,qu.GetData(pDstData,3));
 }
 //队列为空，get数据，期望值WAIT_TIME_OUT
 TEST(FIFOQueue,GetData3)//no values
 {
	 char pDstData[32] = {0};
	 qu.GetData(pDstData,3);
	 ASSERT_EQ(WAIT_TIME_OUT,qu.GetData(pDstData,3, 1));
	 qu.FreeQueue();
 }

 //释放缓冲区
TEST(FIFOQueue, FREE)
{
	ASSERT_TRUE(qu.FreeQueue());
}

//释放缓冲区后，put数据，期望值NO_MEMORY_IN_QUEUE
TEST(FIFOQueue,PutData7)//no memory in queue
{
	char *pSrcData = "123456";
	ASSERT_EQ(NO_MEMORY_IN_QUEUE,qu.PutData(pSrcData,strlen(pSrcData)));
}
//put 5个数据，每隔0.5s取一次数据
unsigned __stdcall PutDataThread_GetData4(void* pArguments)
{
	que.AllocQueue();
	char *pSrcData = "123";
	int i = 0;
	while (i <= 4)
	{
		que.PutData(pSrcData,4);
		i++;
	}
	return 0;

};
TEST(FIFOQueue, GetData4)
{
	HANDLE hThread;
	unsigned threadID;

	hThread = (HANDLE)_beginthreadex( NULL, 0, &PutDataThread_GetData4, NULL, 0, &threadID );

	char pDstData[32] = {0};
	int i = 0;
	while (i <= 4)
	{
		Sleep(500);
		ASSERT_EQ(SUCCESS,que.GetData(pDstData,4));
		i++;
	}

}
//每隔5s put一个数据，get数据没有时间间隔
unsigned __stdcall PutDataThread_GetData5(void* pArguments)
{
	que.AllocQueue();
	char *pSrcData = "123";
	int i = 0;
	while (i <= 4)
	{
		Sleep(500);
		que.PutData(pSrcData,4);
		i++;
	}
	return 0;

};
TEST(FIFOQueue, GetData5)
{
	HANDLE hThread;
	unsigned threadID;

	hThread = (HANDLE)_beginthreadex( NULL, 0, &PutDataThread_GetData5, NULL, 0, &threadID );

	char pDstData[32] = {0};
	int i = 0;
	while (i <= 4)
	{
		ASSERT_EQ(SUCCESS,que.GetData(pDstData,4));
		i++;
	}

}
#endif