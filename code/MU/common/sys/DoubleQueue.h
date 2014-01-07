#ifndef _DOUBLEQUEUE_H_
#define _DOUBLEQUEUE_H_

#include <iostream>
#include <queue>
#include <pthread.h>
using namespace std;

template <class ElemType>
class DoubleQueue
{
public:

	#define NESSSIZE (-1)

	DoubleQueue()
	{
		m_pReadQueue = new queue<ElemType>();
		m_pWriteQueue = new queue<ElemType>();
		pthread_mutex_init(&m_ReadMutex, 0);
		pthread_mutex_init(&m_WriteMutex, 0);

		m_MaxQueueSize = NESSSIZE;
		m_SwapCounter = 0;
	}
	~DoubleQueue()
	{
		delete m_pReadQueue;
		delete m_pWriteQueue;
		pthread_mutex_destroy(&m_ReadMutex);
		pthread_mutex_destroy(&m_WriteMutex);
	}

	queue<ElemType> *m_pReadQueue;
	pthread_mutex_t m_ReadMutex;

	queue<ElemType> *m_pWriteQueue;
	pthread_mutex_t m_WriteMutex;

	int m_MaxQueueSize;
	int m_SwapCounter;
	
	bool push(const ElemType pElem)
	{
		//check args
		
		//ignore the max queue size, just simple to push.
		if(m_MaxQueueSize == NESSSIZE){
			//cout <<"--push()->1"<<endl;
			pthread_mutex_lock(&m_WriteMutex);
			//cout <<"--push()->2"<<endl;
			m_pWriteQueue->push(pElem);
			//cout <<"--push()->3"<<endl;
			pthread_mutex_unlock(&m_WriteMutex);
			//cout <<"--push()->4"<<endl;
			return true;
		}else{
		//focus the max queue size, if reach max size, swap queues.
		//TODO
		}
	}
	bool pop(ElemType *ret)
	{
		//if read queue is empty, swap 
		if(m_pReadQueue->empty()){
			//cout <<"--pop()->1"<<endl;
			swapQueues();
			//cout <<"--pop()->2"<<endl;
		}
		//cout <<"--pop()->1"<<endl;
		if(m_pReadQueue->empty()){
			//cout <<"--pop()->3"<<endl;
			return false;
		}
		//cout <<"--pop()->4"<<endl;
		//somethings remain in read queue, just simple pop it
		pthread_mutex_lock(&m_ReadMutex);
		//cout <<"--pop()->5"<<endl;
		*ret = m_pReadQueue->front();
		//cout <<"--pop()->6"<<endl;
		m_pReadQueue->pop();
		//cout <<"--pop()->7"<<endl;
		pthread_mutex_unlock(&m_ReadMutex);
		//cout <<"--pop()->8"<<endl;
		return true;
	}
	int size()
	{
		return (m_pReadQueue->size() + m_pWriteQueue->size());
	}
	int sizeReadQueue()
	{
		return m_pReadQueue->size();
	}
	int sizeWriteQueue()
	{
		return m_pWriteQueue->size();
	}


	void swapQueues()
	{
		//cout <<"Swap Queues.   ";
		pthread_mutex_lock(&m_WriteMutex);
		//cout <<"--pop()->swapQueues()->1"<<endl;
		pthread_mutex_lock(&m_ReadMutex);
		//cout <<"--pop()->swapQueues()->2"<<endl;
		
		queue<ElemType> *m_tmp = m_pReadQueue;
		//cout <<"--pop()->swapQueues()->3"<<endl;
		m_pReadQueue = m_pWriteQueue;
		//cout <<"--pop()->swapQueues()->4"<<endl;
		m_pWriteQueue = m_tmp;
		//cout <<"--pop()->swapQueues()->5"<<endl;

		m_SwapCounter++;
		//cout <<"--pop()->swapQueues()->6"<<endl;
		//cout <<"ReadQueue:"<<sizeReadQueue()<<"   WriteQueue:"<<sizeWriteQueue()<<"    Count:"<<m_SwapCounter<<endl;

		pthread_mutex_unlock(&m_ReadMutex);
		//cout <<"--pop()->swapQueues()->7"<<endl;
		pthread_mutex_unlock(&m_WriteMutex);
		//cout <<"--pop()->swapQueues()->8"<<endl;
	}
	
};


#endif

