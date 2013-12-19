#ifndef _DOUBLEQUEUE_H_
#define _DOUBLEQUEUE_H_

#include <iostream>
#include <queue>
using namespace std;

template <class ElemType>
class DoubleQueue
{
public:

	#define NESSSIZE (-1)

	DoubleQueue()
	{
		m_pReadQueue = new queue<ElemType*>();
		m_pWriteQueue = new queue<ElemType*>();
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

	queue<ElemType*> *m_pReadQueue;
	pthread_mutex_t m_ReadMutex;

	queue<ElemType*> *m_pWriteQueue;
	pthread_mutex_t m_WriteMutex;

	int m_MaxQueueSize;
	int m_SwapCounter;
	
	bool push(ElemType *pElem)
	{
		//check args
		if(pElem == NULL){
			return false;
		}
		
		//ignore the max queue size, just simple to push.
		if(m_MaxQueueSize == NESSSIZE){
			pthread_mutex_lock(&m_WriteMutex);
			m_pWriteQueue->push(pElem);
			pthread_mutex_unlock(&m_WriteMutex);
		}else{
		//focus the max queue size, if reach max size, swap queues.
		//TODO
		}
	}
	bool pop(ElemType **ret)
	{
		//if read queue is empty, swap 
		if(m_pReadQueue->empty()){
			swapQueues();
		}
		if(m_pReadQueue->empty()){
			return false;
		}
		//somethings remain in read queue, just simple pop it
		pthread_mutex_lock(&m_ReadMutex);
		*ret = m_pReadQueue->pop();
		pthread_mutex_unlock(&m_ReadMutex);
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
		pthread_mutex_lock(&m_WriteMutex);
		pthread_mutex_lock(&m_ReadMutex);
		
		queue<ElemType*> *m_tmp = m_pReadQueue;
		m_pReadQueue = m_pWriteQueue;
		m_pWriteQueue = m_tmp;

		m_SwapCounter++;

		pthread_mutex_lock(&m_ReadMutex);
		pthread_mutex_lock(&m_WriteMutex);
	}
	
};


#endif
