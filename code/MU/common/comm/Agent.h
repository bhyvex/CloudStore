#ifndef AGENT_H_
#define AGENT_H_

#include "SocketAddress.h"
#include "Error.h"

#include <inttypes.h>

struct BaseRequest;
struct InReq;
class Agent
{
public:
    Agent();
    virtual ~Agent();
    //virtual int connect(void);
    virtual int recvData(void);
    virtual int sendData(void);
    virtual int recvReq(BaseRequest *req);
    virtual void writeBack(bool result) {}
    virtual void readBack(InReq &req) {}
    virtual int connectAfter(bool);
    virtual int connect(const SocketAddress &);
    virtual int connect();
    virtual int recycler(void);
    virtual bool isRecycler(void);
    virtual int getOppAddr(SocketAddress &) {
        return SUCCESSFUL;
    }
    virtual int getErrno();
    Agent *ref();
    long getRefCount()const;
    void release();
    int getState()const;
    virtual void setState(int);
    int allowReconnect()const;
    int getConnectTimes()const {
        return m_iConnectTimes;
    }
    void resetConnect() {
        m_iConnectTimes = 0;
    }

    uint64_t getAgentId() const {
        return m_AgentId;
    }

    void setAgentId(uint64_t agentId) {
        m_AgentId = agentId;
    }

protected:
    int m_nRefCount;//maybe it's useless in this project;
    bool m_bIsRecycler;
    int m_iConnect;
    int m_iConnectTimes;

    uint64_t m_AgentId;
};

#endif

