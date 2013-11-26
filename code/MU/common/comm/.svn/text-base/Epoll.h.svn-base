#ifndef __EPOLLEVENTSCHEDULER_H__
#define __EPOLLEVENTSCHEDULER_H__
#include <sys/epoll.h>
#include <sys/time.h>
#include "Error.h"
#include <list>
class EpollEvent;

class Epoll
{
public:
    Epoll();
    virtual ~Epoll(void);
    int getEpollFd()const;
    /*initialize the epoll fds
    **return suc(0) fail(-1)
    */
    int initialize( unsigned int );

    /*
    **get current system time based on millisecond
    */
    const struct timeval& getCurrent(void);

    /*add or change event on fd
    **return suc(0) fail (-1)
    	*/
    int doEvent( EpollEvent* ptr, int fd, int op, unsigned int events);

    /*epoll loop
      **
      */
    void run(void);
    virtual int doTask()
    {
        return SUCCESSFUL;
    }

private:

    struct epoll_event*	  mEpollEvents;
    int	    			  mEpollFd;
    unsigned int 		  mEventSize;
    struct timeval		  mCurrent;
    std::list<int>  mm;

};
#endif

