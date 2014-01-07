#ifndef _THREADPOOLWORKITEM3_H_
#define _THREADPOOLWORKITEM3_H_

#include <stdint.h>
#include "comm/Error.h"

//#include "sys/ThreadPoolDispatcher3.h"


class ThreadPoolWorkItem3
{
public:
	ThreadPoolWorkItem3() {}
	virtual ~ThreadPoolWorkItem3() {}

	virtual int preProcess()
	{
		return SUCCESSFUL;
	}

	virtual int process() = 0;

	virtual int postProcess()
	{
		//g_pDispatcher3->postResult(this);
		return SUCCESSFUL;
	}

	virtual uint64_t getTaskID()
	{
		return m_taskID;
	}

	virtual void setTaskID( uint64_t id )
	{
		m_taskID = id;
	}
private:
	uint64_t m_taskID;
};


#endif

