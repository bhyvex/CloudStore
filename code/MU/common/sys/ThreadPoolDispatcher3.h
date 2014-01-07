#ifndef _THREADPOOLDISPATCHER3_H_
#define _THREADPOOLDISPATCHER3_H_

#include <list>
#include <map>

#include "comm/Agent.h"
#include "comm/EpollEvent.h"
#include "sys/DoubleQueue.h"

class ThreadPoolWorkItem3;

class ThreadPoolDispatcher3 : public Agent
{
public:
	ThreadPoolDispatcher3();
	~ThreadPoolDispatcher3();

	int init();
	void postRequest(ThreadPoolWorkItem3*);

	virtual int recvData();
	virtual int sendData();

	void postResult(const ThreadPoolWorkItem3*);
	
protected:
	int setNonblock(int fd) const;
	
protected:

	//for epoll
	int m_eventfd[2];
	EpollEvent m_readEpollEvent;
	EpollEvent m_writeEpollEvent;
	
};
extern ThreadPoolDispatcher3 *g_pDispatcher3;

/* use ThreadPoolDispatcher3:
 *   ThreadPoolDispatcher3 *g_pDispatcher3 = 
 * 		(AgentManager::getInstance())->createAgent<ThreadPoolDispatcher3>();
 *
 */


#endif

