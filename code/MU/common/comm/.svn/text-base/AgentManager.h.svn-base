#ifndef _AGENTMANAGER_H
#define _AGENTMANAGER_H
#include "sys/Singleton.h"
#include "TCPAgent.h"
#include <map>
#include <tr1/memory>
using namespace std;
class TCPAgent;

class AgentManager: public Singleton<AgentManager>
{
protected:
    AgentManager();
    ~AgentManager();
    friend class Singleton<AgentManager>;
public:
    int addAgent(TCPAgent *, const string &);
    int deleteAgent(const TCPAgent *, const string &);
    TCPAgent *getAgent(const string &);
    template<typename Type> Type *createAgent(Epoll *pEpoll) {
        Type *ptr = new Type(pEpoll);
        return ptr;
    }

    template<typename Type> Type *createAgent(const SocketAddress &addr, Epoll *pEpoll) {
        Type *ptr = new Type(addr, pEpoll);
        return ptr;
    }

    template<typename Type> Type *createAgent(const TCPSocket &sock, const SocketAddress &addr, Epoll *pEpoll) {
        Type *ptr = new Type(sock, addr, pEpoll);
        return ptr;
    }

    void recyclerAgent(Agent *pAgent) {
        if (pAgent != NULL) {
            pAgent->recycler();
            delete pAgent;
        }
    }

    int updateAddr(const string &, const tr1::shared_ptr<SocketAddress> &);
    int getAddr(const string &, SocketAddress &);

private:
    map<string, TCPAgent *> m_TCPAgentMap;
    map<string, tr1::shared_ptr<SocketAddress> > m_AddrMap;
};
#endif

