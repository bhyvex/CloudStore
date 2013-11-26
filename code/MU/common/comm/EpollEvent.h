#ifndef __EPOLLEVENT_H__
#define __EPOLLEVENT_H__
#include "TCPSocket.h"
#include "Agent.h"
class Agent;
class Epoll;
class EpollEvent
{
    typedef struct
    {
        unsigned int event;
        unsigned int rlock;
        unsigned int wlock;
        bool		    read;
        bool 	    write;
    } event_st;
public:
    EpollEvent()
    {
        mEpollEvent.event = 0;
        mEpollEvent.rlock = 0;
        mEpollEvent.wlock = 0;
        mHandler = NULL;
        m_Epoll = NULL;
        mFd = -1;

    }
    ~EpollEvent(void)
    {
        mHandler = NULL;
        m_Epoll = NULL;
    }

    /*
    **set the epoll fd
    */
    void setFd( int fd)
    {
        mFd = fd;
        mEpollEvent.event = 0;
        mEpollEvent.rlock = 0;
        mEpollEvent.wlock = 0;
    }
    void setEpoll(Epoll *epl)
    {
        m_Epoll = epl;
    }
    Epoll* getEpoll()
    {
        return m_Epoll;
    }
    /*
    **set handler
    */
    void setHandler(Agent* handler)
    {
        mHandler = handler;
    }

    Agent* getHandler(void)
    {
        return mHandler;
    }

    /*open the read event on sock
    **retun suc(0) error(-1)
    */
    int openRevent(void);
    /*open the write event on  sock
    **return suc(0) error(-1)
    */
    int openWevent(void);
    /*close the read event on sock
    **return suc(0) error(-1)
    */
    int closeRevent(void);
    /*close the write event on sock
    **return suc(0) error(-1)
    */
    int closeWevent(void);
    /*register the read event on sock
    **return suc(0) error(-1)
    */
    int registerREvent(void);
    /*register the write event on sock
    **return suc(0) error(-1)
    */
    int registerWEvent(void);
    /*register the read and write event on sock
    **return suc(0) error(-1)
    */
    int registerRWEvents(void);

    int unregisterRWEvents(void);

protected:
    event_st 	   mEpollEvent;
    Agent* 	   mHandler;
    Epoll      *m_Epoll;
    int 		   mFd;
};

#endif

