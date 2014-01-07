#ifndef _THREADPOOL3_H_
#define _THREADPOOL3_H_

#include "sys/Thread.h"
#include "sys/ThreadPoolWorkItem3.h"
#include "sys/Mutex.h"
#include "sys/DoubleQueue.h"
#include <iostream>
#include <map>

using std::cout;
using std::endl;

// 最大空闲线程数
#define THREADPOOL3_THREAD_MAX_IDLE_DEFAULT 15
// 最大线程总数
#define THREADPOOL3_THREAD_MAX_TOTAL_DEFAULT 60

class ThreadPool3
{
public:
	ThreadPool3(int maxIdle = THREADPOOL3_THREAD_MAX_IDLE_DEFAULT,
			int maxTotal = THREADPOOL3_THREAD_MAX_TOTAL_DEFAULT);
	~ThreadPool3();

	int postRequest( const ThreadPoolWorkItem3* pWorkItem );
	int postRequest( int threadID, const ThreadPoolWorkItem3* pWorkItem );
	int postResult( const ThreadPoolWorkItem3* pWorkItem );
	int postResult( int threadID, const ThreadPoolWorkItem3* pWorkItem );
	
	ThreadPoolWorkItem3* popResultItem(int threadID);
			
	int start();
	int stop();

	int getMaxIdle();
	
	
protected:
	ThreadPool3( const ThreadPool3& tp );
	ThreadPool3& operator=( const ThreadPool3& tp );

	class EventHandlerThread : public Thread
	{
	public:
		EventHandlerThread();

		int DispatchItem(const ThreadPoolWorkItem3 *item);
		int ResultItem(const ThreadPoolWorkItem3 *item);

		
	protected:
		virtual void run();
		void SetNonblock(int fd);

	public:
		DoubleQueue<ThreadPoolWorkItem3 *>  m_RequestQueue;
		DoubleQueue<ThreadPoolWorkItem3 *>  m_ResultQueue;
	};

	class ThreadExitWorkItem : public ThreadPoolWorkItem3
	{
	public:
        int process();
	};
	
private:
	int m_nMaxIdle;
	int m_nMaxTotal;

	std::map<int, EventHandlerThread*> m_threadMap;
	Mutex m_mutex;  // to protect m_threadMap;

	int m_nNextThread;


};
extern ThreadPool3 *g_pThreadPool3;





#endif
