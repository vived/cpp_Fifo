#pragma once

/*
	����:FIFOQueue.h
	����ܣ��˿�Ϊ�Ƚ��ȳ�������п⣬�������첽��д����
	�汾��1.0
	����:zhangfang 20140220
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

/*����ʵ��֮�� �������AllocQueue���������������ж��з���*/
class  DllImportOrExport CFIFOQueue
{
public:
	/*szEveryBufSize: ÿһ���С
	  szCount:������ٿ�
	  ע�⣺���������ܻᵼ�±���
	*/
	CFIFOQueue(size_t szEveryBufSize, size_t szCount);
	~CFIFOQueue();
public:
	/*������ô˺������з��䣬������б��޷�ʹ��*/
	bool AllocQueue();
	bool FreeQueue();
	/*�������ݣ��������������ȴ������szWaitSecs��Ϊ0����ȴ���������ʧ�ܷ���*/
	int PutData(char *pSrcData, size_t szLen, size_t szWaitSecs = 0);
	/*��ȡ���ݣ��������Ϊ����ȴ�*/
	int GetData(char *pDstData, size_t szRecvLen, size_t szWaitSecs = 0);
protected:
private:
	CMemPool *m_pPool;
	size_t m_szBufSize;
	size_t m_szCount;
};