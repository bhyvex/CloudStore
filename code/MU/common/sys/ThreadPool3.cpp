#include <cstdlib>
#include <iostream>
#include <stdio.h>

#include "sys/ThreadPool3.h"
#include "log/log.h"
#include "util/util.h"
#include "comm/Error.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

ThreadPool3::EventHandlerThread::
EventHandlerThread(ThreadPool3 *pool):
	m_pThreadPool(pool)
{
	if( NULL == pool)
	{
		DEBUG_LOG( "NULL pointer argument." );
	}

}

int ThreadPool3::EventHandlerThread::
DispatchItem(const ThreadPoolWorkItem3 *item)
{
	ThreadPoolWorkItem3 *pItem = const_cast<ThreadPoolWorkItem3 *>(item);
	bool ret = m_RequestQueue.push(pItem);
	if(ret != true)
	{
		return FAILED;
	}

	return SUCCESSFUL;
}

int ThreadPool3::EventHandlerThread::
ResultItem(const ThreadPoolWorkItem3 *item)
{
	ThreadPoolWorkItem3 *pItem = const_cast<ThreadPoolWorkItem3 *>(item);
	bool ret = m_ResultQueue.push(pItem);
	if(ret != true)
	{
		return FAILED;
	}

	return SUCCESSFUL;
}





void ThreadPool3::EventHandlerThread::
run()
{
	ThreadPoolWorkItem3 *pWorkItem = NULL;

	for(; ; )
	{

		bool ret = m_RequestQueue.pop(&pWorkItem);

		pthread_testcancel();

		if(ret != true)
		{
			DEBUG_LOG( "pop error:queue is empty.");
			::sleep(1);
			continue;
		}

		pWorkItem->preProcess();
        pWorkItem->process();
        pWorkItem->postProcess();
	}
}



ThreadPool3::ThreadPool3( int maxIdle, int maxTotal ) :
	m_nMaxIdle( maxIdle ),
    m_nMaxTotal( maxTotal ),
    m_nNextThread(0)
{
    if ( maxIdle <= 0 ||
            maxTotal <= 0 ||
            maxIdle < maxTotal )
    {
        m_nMaxIdle = THREADPOOL3_THREAD_MAX_IDLE_DEFAULT;
        m_nMaxTotal = THREADPOOL3_THREAD_MAX_TOTAL_DEFAULT;
    }
}


int ThreadPool3::postRequest(const ThreadPoolWorkItem3 * pWorkItem)
{	
	m_nNextThread = (m_nNextThread + 1) % m_nMaxIdle;

	int rt = postRequest(m_nNextThread, pWorkItem);
	
	return rt;
}

int ThreadPool3::postRequest(int threadID, const ThreadPoolWorkItem3 * pWorkItem)
{
	EventHandlerThread *eventhandlerthread = m_threadMap[threadID];
	if(eventhandlerthread == NULL)
	{
		return FAILED;
	}

	int rt = eventhandlerthread->DispatchItem(pWorkItem);

	return rt;
}

int ThreadPool3::postResult(const ThreadPoolWorkItem3 * pWorkItem)
{	
	int rt = postRequest(m_nNextThread, pWorkItem);
	
	return rt;
}

int ThreadPool3::postResult(int threadID, const ThreadPoolWorkItem3 * pWorkItem)
{
	EventHandlerThread *eventhandlerthread = m_threadMap[threadID];
	if(eventhandlerthread == NULL)
	{
		return FAILED;
	}

	int rt = eventhandlerthread->ResultItem(pWorkItem);

	return rt;
}


ThreadPoolWorkItem3* 
ThreadPool3::
popResultItem(int threadID)
{

	EventHandlerThread *eventhandlerthread = m_threadMap[threadID];
	if(eventhandlerthread == NULL)
	{
		return NULL;
	}

	
	ThreadPoolWorkItem3 *pWorkItem = NULL;

	bool ret = eventhandlerthread->m_ResultQueue.pop(&pWorkItem);	
	if(ret != true)
	{
		return NULL;
	}

	return pWorkItem;	
}


int ThreadPool3::start()
{
	for( int i = 0; i < m_nMaxIdle; ++i)
	{
		m_threadMap[i] = new EventHandlerThread(this);
	}
	
	for( int i = 0; i < m_nMaxIdle; ++i )
	{
		int rt = m_threadMap[i]->start();
		if(-1 == rt)
		{
			DEBUG_LOG( "Error occurred whilst starting a thread." );
            return FAILED;
		}else{
			INFO_LOG( "started a thread." );
		}
	}

	return SUCCESSFUL;
}

int ThreadPool3::stop()
{
	INFO_LOG("threadpool stop");
	if ( !m_threadMap.empty() )
	{
		for( int i = 0; i < m_nMaxIdle; ++i )
		{
			delete ( m_threadMap[i]);
		}
	}

	return SUCCESSFUL;
}

int ThreadPool3::getMaxIdle()
{
	return m_nMaxIdle;
}



ThreadPool3::~ThreadPool3()
{
}

int ThreadPool3::ThreadExitWorkItem::
process()
{
	return SUCCESSFUL;
}

