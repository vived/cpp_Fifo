#include "stdafx.h"


//#include "fifoqueue.h"
#include "../FIFOQueue/FIFOQueue.h"

 #include <windows.h>
 #include <process.h>

#if 0

static CFIFOQueue qu(10,2);
static CFIFOQueue que(10,3);

//û�з��䣬ֱ��ʹ��,����ֵNO_MEMORY_IN_QUEUE
TEST(FIFOQueue,PutData1)
{
	char *pSrcData = "123";
	ASSERT_EQ(NO_MEMORY_IN_QUEUE,qu.PutData(pSrcData,strlen(pSrcData)));
}
//���仺����
TEST(FIFOQueue, Allocate)
{
	//EXPECT_EQ(true, qu.AllocQueue());
	ASSERT_TRUE(qu.AllocQueue());
}
//���������put���ݣ�ֱ��get����,����ֵWAIT_TIME_OUT
 TEST(FIFOQueue,GetData1)//no values
 {
	 char pDstData[32] = {0};
 	ASSERT_EQ(WAIT_TIME_OUT,qu.GetData(pDstData,3, 1));
 }
//����put���ݣ�����ֵSUCCESS
TEST(FIFOQueue, PutData2)
{
	char *pSrcData = "123";
	ASSERT_EQ(SUCCESS,qu.PutData(pSrcData,strlen(pSrcData)));
}
//put�����ݳ������ݿ�Ĵ�С,����ֵOUT_OF_RANGE
TEST(FIFOQueue,PutData3)
{
	char *pSrcData = "98765432101";
	ASSERT_EQ(OUT_OF_RANGE,qu.PutData(pSrcData,strlen(pSrcData)));
}
//put���ݣ���������Ŀ���������ֵWAIT_TIME_OUT
TEST(FIFOQueue,PutData4)
{
	char *pSrcData = "9876";
 	qu.PutData(pSrcData,strlen(pSrcData));

	pSrcData = "456";
	ASSERT_EQ(WAIT_TIME_OUT,qu.PutData(pSrcData,strlen(pSrcData),1));//��������Ŀ���
}
//put���ݣ���������Ŀ������ȴ�2s���ڵȴ��ڼ�get���ݣ��ٽ���put���ݣ�����ֵSUCCESS
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

//put���ݣ���������Ŀ������ȴ�2s���ڵȴ��ڼ�get���ݣ��ٽ���put����
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
	ASSERT_EQ(WAIT_TIME_OUT,qu.PutData(pSrcData, strlen(pSrcData), 1));//�ȴ�1s�󷵻أ�����ֵ��WAIT_TIME_OUT

	CloseHandle( hThread );
}
//�����������ݣ�����get���ݣ�����ֵSUCCESS
 TEST(FIFOQueue,GetData2)
 {
 	char pDstData[32] = {0};
 	ASSERT_EQ(SUCCESS,qu.GetData(pDstData,3));
 }
 //����Ϊ�գ�get���ݣ�����ֵWAIT_TIME_OUT
 TEST(FIFOQueue,GetData3)//no values
 {
	 char pDstData[32] = {0};
	 qu.GetData(pDstData,3);
	 ASSERT_EQ(WAIT_TIME_OUT,qu.GetData(pDstData,3, 1));
	 qu.FreeQueue();
 }

 //�ͷŻ�����
TEST(FIFOQueue, FREE)
{
	ASSERT_TRUE(qu.FreeQueue());
}

//�ͷŻ�������put���ݣ�����ֵNO_MEMORY_IN_QUEUE
TEST(FIFOQueue,PutData7)//no memory in queue
{
	char *pSrcData = "123456";
	ASSERT_EQ(NO_MEMORY_IN_QUEUE,qu.PutData(pSrcData,strlen(pSrcData)));
}
//put 5�����ݣ�ÿ��0.5sȡһ������
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
//ÿ��5s putһ�����ݣ�get����û��ʱ����
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