#include <unistd.h>
#include <fcntl.h>

#include "sys/ThreadPoolDispatcher3.h"
#include "sys/ThreadPool3.h"
#include "log/log.h"
#include "comm/BaseTask.h"
#include "comm/TaskManager.h"
#include "comm/BaseHeader.h"
#include "util/util.h"
#include "comm/Epoll.h"

#include <sys/socket.h>
#include <sys/types.h>

ThreadPoolDispatcher3::ThreadPoolDispatcher3()
{
	if( NULL == g_pEpoll ||
		NULL == g_pThreadPool3 )
	{
		DEBUG_LOG(
            "NULL pointer argument passed to ThreadPoolDispatcher()." );
        return;
	}


	//start eventfd
	int rt = socketpair( AF_UNIX, SOCK_STREAM, 0, m_eventfd);
	if(-1 == rt)
	{
		DEBUG_LOG( "socketpair() error." );
	}

	setNonblock(m_eventfd[0]);//main thread
	setNonblock(m_eventfd[1]);//work thread


	//set EPOLLIN on main thread's handler
	m_readEpollEvent.setFd(m_eventfd[0]);
	m_readEpollEvent.setHandlerID( getID());
	m_readEpollEvent.registerREvent();

	// set state
    m_iConnect = CONNECTED;

    //trigger main thread's handler to read
    rt = ::write(m_eventfd[0], "trigger to read", sizeof("trigger to read"));
    if(-1 == rt)
	{
		DEBUG_LOG( "write() error." );
	}
	
    rt = ::write(m_eventfd[1], "trigger to read", sizeof("trigger to read"));
    if(-1 == rt)
	{
		DEBUG_LOG( "write() error." );
	}

}

ThreadPoolDispatcher3::~ThreadPoolDispatcher3()
{
    try
    {
    	m_readEpollEvent.unregisterRWEvents();
    }
    catch ( ... )
    {
    }
}

int ThreadPoolDispatcher3::
init()
{
	return SUCCESSFUL;
}

void
ThreadPoolDispatcher3::postRequest(
	ThreadPoolWorkItem3* pWorkItem)
{
	g_pThreadPool3->postRequest(pWorkItem);
}

void 
ThreadPoolDispatcher3::postResult(const ThreadPoolWorkItem3* pWorkItem)
{
	g_pThreadPool3->postResult(pWorkItem);
}

int
ThreadPoolDispatcher3::recvData()
{
	ThreadPoolWorkItem3* pWorkItem = NULL;


	for (int i = 0; i < g_pThreadPool3->getMaxIdle(); i++)
	{
		pWorkItem = g_pThreadPool3->popResultItem(i);
		if(pWorkItem == NULL){
			break;
		}

		BaseTask* pTask = TaskManager::getInstance()->get(
                              pWorkItem->getTaskID() );

        if ( NULL == pTask )
        {
            // task已析构
            delete pWorkItem;
            pWorkItem = NULL;
        }
        else
        {
            // task正常
            pTask->recvWorkItem( pWorkItem );
        }
    }

    return SUCCESSFUL;
}

int
ThreadPoolDispatcher3::sendData()
{
	return SUCCESSFUL;
}

int ThreadPoolDispatcher3::setNonblock(int fd) const
{
    int val;

    if ( ( val = fcntl( fd, F_GETFL, 0 ) ) < 0 )
    {
        ERROR_LOG( "TCPSocket::setNonBlock::fcntl-F_GETFL" );
        return val;
    }
    val |= O_NONBLOCK;
    if ( fcntl( fd, F_SETFL, val ) < 0 )
    {
        ERROR_LOG( "TCPSocket::setNonBlock:fcntl" );
        return FAILED;
    }

    return SUCCESSFUL;
}

