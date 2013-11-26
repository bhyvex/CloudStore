#ifndef BUFFERV_H__
#define BUFFERV_H__
#include <list>
#include <sys/uio.h>
#include <netinet/in.h>
#include "TCPSocket.h"
#include "BaseHeader.h"
#include "BaseTask.h"
#include "BaseReq.h"
#include "Agent.h"
#include <cstring>
using namespace std;
class TCPAgent;
//class BaseTask;
class UDPSocket;
class BufferManager
{
    struct iov_req
    {
        iov_req() : mComplete(true), mTask(NULL)
        {}
        iov_req(char *buffer, unsigned int len , BaseTask *task = NULL)
            : mComplete(true),
              mTask(task)
        {
            mIov.iov_base = buffer;
            mIov.iov_len = len;
            bzero(&mUDPAddr, sizeof(struct sockaddr_in));
        }

        struct iovec 	mIov;
        bool			mComplete;
        struct sockaddr_in mUDPAddr;
        BaseTask *mTask;
    };

public:
    BufferManager( void );
    ~BufferManager( void );
    /*copy buffer to mIov
    **return suc( 0) errror(-1)
    */
//	int writeDynamic(char*, int);//for Agent
    int writeDynamic(char*, unsigned int  len ,BaseTask* task);//for BaseTask
    int writeDynamic(char*, unsigned int  len ,SocketAddress &,BaseTask* task);//for BaseTask
//	int writeDynamic( char*, int  len ,Agent *agent);
//	int write ( char*, int ,struct sockaddr_in &);
//	int parseData();
    /*write to sock
    **return suc( 0) error(-1)
    */
    int write( TCPSocket& );
    int write( UDPSocket& );
    /*write to sock and most len data
    **return suc(0) error(-1)
    */
    //int Write( TCPSocket&, unsigned int);
    /*return buffer data len
    **
    */
    //int readDynamic(BaseTask *);
    int read(TCPSocket&);
    int read(UDPSocket&);
    unsigned int getLength(void )const;
    int clear(void);
    //add by thunder 2008-1-3
    //	void DelQueueData();
    //add end
    void setAgent(Agent *agent);
    void handleWriteError();
private:
    void* 			mLastIov;
    Agent *m_Agent;
    unsigned int m_iOffset;
    unsigned int m_iLen;
    bool m_bReadHead;
    bool m_bInit;
    std::list<iov_req> mIovList;
    struct InReq m_InReq;

    // limit of iovec count
    int m_IovLimit;
};
#endif
