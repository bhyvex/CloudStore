#include "BufferManager.h"
//#include "TCPSocket.h"
#include "UDPSocket.h"
#include "TCPAgent.h"
#include <errno.h>
#include <iostream>
#include "Error.h"
#include "DevLog/DevLog.h"


#include <unistd.h>
#include <limits.h>

#define __need_IOV_MAX

#ifndef IOV_MAX

#define IOV_MAX 1024

#endif


using namespace std;
extern DevLog *g_pDevLog;

BufferManager::BufferManager()
    : mLastIov(NULL),
      m_Agent(NULL),
      m_iOffset(0),
      m_iLen(0),
      m_bReadHead(true),
      m_bInit(true)
{
    m_IovLimit = IOV_MAX;
    //cout << "IOV_MAX: " << m_IovLimit << endl;
}

BufferManager::~BufferManager()
{
    try {
        m_Agent = NULL;
        list<iov_req>::iterator aIt = mIovList.begin();

        while (aIt != mIovList.end()) {
            if ((*aIt).mComplete) {
                if ((*aIt).mIov.iov_base) {
                    delete [](char *)(*aIt).mIov.iov_base;
                    (*aIt).mIov.iov_base = NULL;
                }
            }

            ++aIt;
        }

        mIovList.clear();

        if (mLastIov != NULL) {
            delete [](char *)mLastIov;
            mLastIov = NULL;
        }

    } catch (...) {
        DEV_LOG(LERROR, OUT_BOTH, "catch exception in ~BufferManager");
    }
}
int BufferManager::clear(void)
{
//  cout<<"the clear is called"<<endl;
    list<iov_req>::iterator aIt = mIovList.begin();

    while (aIt != mIovList.end()) {
        if ((*aIt).mComplete)
            if ((*aIt).mIov.iov_base) {
                delete [](char *)(*aIt).mIov.iov_base;
                (*aIt).mIov.iov_base = NULL;
            }

        ++aIt;
    }

    mIovList.clear();

    if (mLastIov != NULL) {
        delete [](char *)mLastIov;
        mLastIov = NULL;
    }

    return 0;
}
void BufferManager::setAgent(Agent *agent)
{
    this->m_Agent = agent;
}

int BufferManager::writeDynamic(char *buffer, unsigned int len , BaseTask *task = NULL)
{
    struct iov_req iov(buffer, len, task);
    mIovList.push_back(iov);
    return SUCCESSFUL;
}

int BufferManager::writeDynamic
(
    char *buffer,
    unsigned int len,
    SocketAddress &addr,
    BaseTask *task = NULL
)
{
    struct iov_req iov;
    iov.mComplete = true;
    iov.mIov.iov_base = buffer;
    iov.mIov.iov_len = len;

    if (!addr.getAddr(iov.mUDPAddr)) {
        return FAILED;
    }

    iov.mTask = task;
    mIovList.push_back(iov);
    return SUCCESSFUL;
}

int BufferManager::write(UDPSocket &sock)
{
    list<iov_req>::iterator aIt, pIt;
    aIt = mIovList.begin();
    int ret = -1;
    int download = 0;

    for (; aIt != mIovList.end();) {
        pIt = aIt;
        ret = sock.writeSocket((char *)((*pIt).mIov.iov_base),
                               (*pIt).mIov.iov_len, (*pIt).mUDPAddr);

        if (ret < 0) {
            if (errno == EINTR) {
                continue;

            } else if (errno == EWOULDBLOCK) {
                return SUCCESSFUL;

            } else {
                DEV_LOG(LERROR, OUT_BOTH, "BufferManager::write(UDPSocket& sock)");

                if ((*pIt).mTask != NULL) {
                    (*pIt).mTask->writeBack(false);

                } else {
                    if (m_Agent != NULL) {
                        m_Agent->writeBack(false);

                    } else {
                        DEV_LOG(LERROR, OUT_BOTH, "the BufferManager don't relate a Agent");
                    }
                }

                return FAILED;
            }
        }

        download += ret;

        if ((unsigned int)ret < (*pIt).mIov.iov_len) { //in linux, the type of iovec.iov_len is size_t, not int(in windows)
            (*pIt).mIov.iov_base = (void *)((char *)((*pIt).mIov.iov_base) + ret);
            (*pIt).mIov.iov_len -= (unsigned int)ret;
            mLastIov = (*pIt).mIov.iov_base;
            (*pIt).mComplete = false;
            break;

        } else {
            if ((*pIt).mComplete) {
                if ((*pIt).mIov.iov_base) {
                    delete [](char *)(*pIt).mIov.iov_base;
                    (*pIt).mIov.iov_base = NULL;
                }

            } else {
                if (mLastIov != NULL) {
                    delete [](char *)mLastIov;
                    mLastIov = NULL;
                }
            }

            ++aIt;
            mIovList.erase(pIt);

            if ((*pIt).mTask != NULL) {
                (*pIt).mTask->writeBack(true);

            } else {
                if (m_Agent != NULL) {
                    m_Agent->writeBack(true);

                } else {
                    DEV_LOG(LERROR, OUT_BOTH, "the BufferManager don't relate a Agent");
                    return FAILED;
                }
            }
        }

    }

    return download;
}

int BufferManager::write(TCPSocket &sock)
{
    unsigned int reqlen = (unsigned int)mIovList.size();

    if (reqlen > m_IovLimit) {
        reqlen = m_IovLimit;
    }

    struct iovec outdata[ reqlen ];

    memset(outdata, 0, reqlen * sizeof(struct iovec));

    list<iov_req>::iterator aIt , pIt;

    aIt = mIovList.begin();

    unsigned int num = 0;

    while (aIt != mIovList.end() && num < m_IovLimit) {
        outdata[num].iov_base = (*aIt).mIov.iov_base;
        outdata[num].iov_len = (*aIt).mIov.iov_len;
        ++num;
        ++aIt;
    }

    int ret = -1;

    while ((ret = sock.writevSocket(outdata, reqlen)) < 0) {
        if (errno == EINTR) {
            continue;
        }

        if (errno != EWOULDBLOCK) {
            handleWriteError();
            return FAILED;
        }
    }

    int download = ret;
    aIt = mIovList.begin();

    while (ret > 0 && aIt != mIovList.end()) {
        pIt = aIt++;

        if ((unsigned int)ret >= (*pIt).mIov.iov_len) {
            ret -= (*pIt).mIov.iov_len;

            if ((*pIt).mComplete) {
                if ((*pIt).mIov.iov_base) {
                    delete [](char *)(*pIt).mIov.iov_base;
                    (*pIt).mIov.iov_base = NULL;
                }

            } else {
                if (mLastIov != NULL) {
                    delete [](char *)mLastIov;
                    mLastIov = NULL;
                }
            }

            if ((*pIt).mTask != NULL) {
                (*pIt).mTask->writeBack(true);

            } else {
                if (m_Agent != NULL) {
                    m_Agent->writeBack(true);

                } else {
                    DEV_LOG(LERROR, OUT_BOTH, "the BufferManager don't relate a Agent");
                    return FAILED;
                }
            }

            mIovList.erase(pIt);

        } else {
            if ((*pIt).mComplete) {
                mLastIov = (*pIt).mIov.iov_base;
                (*pIt).mComplete = false;
            }

            (*pIt).mIov.iov_base = (void *)((char *)((*pIt).mIov.iov_base) + ret);
            (*pIt).mIov.iov_len -= (unsigned int)ret;
            break;
        }
    }

    return download;
}


unsigned int BufferManager::getLength(void)const
{
    return (unsigned int)mIovList.size();

}


int BufferManager::read(TCPSocket &sock)
{
    int ret;

    if (m_bInit) {
        m_InReq.ioBuf = NULL;
        memset(&m_InReq.m_msgHeader, 0, HEADER_SIZE);
        m_bInit = false;
        m_iLen = HEADER_SIZE;
    }

    if (m_bReadHead) {
        if ((ret = sock.readSocket(((char *)(&(m_InReq.m_msgHeader))) + m_iOffset, \
                                   HEADER_SIZE - m_iOffset)) < 0) {
            if (!((errno == EWOULDBLOCK) || (errno == EINTR))) {
                DEV_LOG(LERROR, OUT_BOTH, "BufferManager::read()");
                return FAILED;
            }

            return SUCCESSFUL;

        } else if (ret == 0) {
            DEV_LOG(LERROR, OUT_BOTH, "the opposit close the connection");
            return FAILED;
        }

        m_iOffset += (unsigned int)ret;

        if (m_iOffset == HEADER_SIZE) {
            m_iOffset = 0;
            m_iLen = m_InReq.m_msgHeader.length;

            if (m_iLen > 0) {
                m_InReq.ioBuf = new char[m_iLen + 1];
                m_bReadHead = false;

            } else if (m_iLen == 0) {
                if (m_Agent != NULL) {
                    m_Agent->readBack(m_InReq);
                    this->m_bInit = true;
                }

                this->m_bReadHead = true;
                return SUCCESSFUL;

            } else {
                m_bReadHead = true;
                DEV_LOG(LERROR, OUT_BOTH, "the data length is not illegal");
                return FAILED;
            }
        }
    }

    if (!m_bReadHead) {
        if ((ret = sock.readSocket(m_InReq.ioBuf + m_iOffset, \
                                   m_iLen - m_iOffset)) < 0) {
            if (!((errno == EWOULDBLOCK) || (errno == EINTR))) {
                DEV_LOG(LERROR, OUT_BOTH, "readSocket in BufferMananger");
                return FAILED;
            }

            return SUCCESSFUL;

        } else if (ret == 0) {
            DEV_LOG(LERROR, OUT_BOTH, "the opposit close the connection in readData");
            return FAILED;
        }

        m_iOffset += (unsigned int)ret;

        if (m_iOffset == m_iLen) {
            m_InReq.ioBuf[m_iLen] = 0;
            m_iOffset = 0;
            m_iLen = 0;
            this->m_bReadHead = true;

            if (m_Agent != NULL) {
                m_Agent->readBack(m_InReq);
                this->m_bInit = true;
            }

            if (m_InReq.ioBuf != NULL) {
                delete [] m_InReq.ioBuf;
                m_InReq.ioBuf = NULL;
            }

            return SUCCESSFUL;
        }
    }

    return (int)(m_iLen - m_iOffset);
}

int BufferManager::read(UDPSocket &sock)
{
    int ret;
    m_InReq.ioBuf = NULL;
    memset(&m_InReq.m_msgHeader, 0, HEADER_SIZE);
    m_iLen = 1024;
    char *buf = new char[m_iLen];

    if ((ret = sock.readSocket(buf, m_iLen, m_InReq.oppoAddr)) < 0) {
        return FAILED;

    } else if ((unsigned int)ret < HEADER_SIZE) {
        DEV_LOG_ERROR("BufferManager::read(UDPSocket &sock) Error, readNum < HEADER_SIZE");
        return FAILED;
    }

    memcpy(&m_InReq.m_msgHeader, buf, HEADER_SIZE);
    m_InReq.ioBuf = new char[ret - HEADER_SIZE + 1];
    memset(m_InReq.ioBuf, 0, ret - HEADER_SIZE + 1);

    if (m_InReq.m_msgHeader.length != (ret - HEADER_SIZE)) {
        DEV_LOG_ERROR("BufferManager::read(UDPSocket &sock) Error, date length error");
    }

    memcpy(m_InReq.ioBuf, buf + HEADER_SIZE, ret - HEADER_SIZE);
    m_Agent->readBack(m_InReq);
    m_bInit = true;
    return SUCCESSFUL;
    /*
        int ret;
        if(m_bInit)
        {
            m_InReq.ioBuf=NULL;
            memset(&m_InReq.m_msgHeader,0,HEADER_SIZE);
            m_bInit = false;
            m_iLen = 1000000;
    //        m_iLen = HEADER_SIZE;
        }
        char* buf = new char[m_iLen];
        if( (ret = sock.readSocket(buf, m_iLen, m_InReq.oppoAddr)) < 0)
        {
            return FAILED;
        }
        cout << "ret = " << ret << endl;
        memcpy(&m_InReq.m_msgHeader, buf, HEADER_SIZE);
        cout << m_InReq.m_msgHeader.length << endl;
        m_InReq.ioBuf = new char[ret - HEADER_SIZE + 1];
        memset(m_InReq.ioBuf, 0, ret - HEADER_SIZE + 1);
        memcpy(m_InReq.ioBuf, buf + HEADER_SIZE, ret - HEADER_SIZE);
        m_Agent->readBack(m_InReq);
        m_bInit = true;
        return SUCCESSFUL;

        if(m_bReadHead)
        {
            if((ret = sock.readSocket(((char*)(&m_InReq.m_msgHeader))+m_iOffset,
                                      m_iLen-m_iOffset,m_InReq.oppoAddr))<0)
            {
                if( !((EINTR == errno) || (EWOULDBLOCK == errno)))
                {
                    DEV_LOG(LERROR, OUT_BOTH, "BufferManager::readUDPSocket ");
                    return FAILED;
                }
                else
                {
                    return SUCCESSFUL;
                }
            }
            m_iOffset+=(unsigned int)ret;
            if(HEADER_SIZE == m_iOffset)
            {
                m_bReadHead = false;
                m_iOffset = 0;
                m_iLen=m_InReq.m_msgHeader.length;
                if( m_iLen > 0)
                {
                    m_InReq.ioBuf = new char[m_iLen+1];
                }
                else if(m_iLen == 0)
                {
                    if(m_Agent != NULL)
                    {
                        m_Agent->readBack(m_InReq);
                    }
                    m_bInit = true;
                    m_bReadHead = true;
                    return SUCCESSFUL;
                }
                else
                {
                    DEV_LOG(LERROR, OUT_BOTH, "the data length invalid in BufferManager:readUDP");
                    return FAILED;
                }
            }
        }
        if(!m_bReadHead)
        {
            if((ret =sock.readSocket(m_InReq.ioBuf+m_iOffset,m_iLen, m_InReq.oppoAddr))<0)
            {
                if( !((EINTR == errno) || (EWOULDBLOCK == errno)))
                {
                    DEV_LOG(LERROR, OUT_BOTH, "BufferManager::readUDPSocket ");
                    return FAILED;
                }
                return SUCCESSFUL;
            }
            m_iOffset+=(unsigned int)ret;
            if(m_iOffset == m_iLen)
            {
                m_InReq.ioBuf[m_iLen]=0;
                m_iOffset =0;
                m_iLen =0;
                m_bReadHead=true;
                m_bInit = true;
                if(m_Agent != NULL)
                {
                    m_Agent->readBack(m_InReq);
                }
                return SUCCESSFUL;
            }
        }
        return (int)(m_iLen - m_iOffset);
    */
}

void BufferManager::handleWriteError()
{
    list<iov_req>::iterator aIt = mIovList.begin();

    while (aIt != mIovList.end()) {
        if ((*aIt).mTask != NULL) {
            (*aIt).mTask->writeBack(false);

        } else {
            if (m_Agent != NULL) {
                m_Agent->writeBack(false);

            } else {
                DEV_LOG(LERROR, OUT_BOTH, "the BufferManager don't relate a Agent");
            }
        }

        ++aIt;
    }
}
