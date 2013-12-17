#include "AgentManager.h"
#include <iostream>
#include <string>
#include "Error.h"
#include "DevLog/DevLog.h"
using namespace std;
extern DevLog *g_pDevLog;

AgentManager::AgentManager()
{
}

AgentManager::~AgentManager()
{
}

int AgentManager::addAgent(TCPAgent* pAgent, const string& AgentType)
{
    pair<map<string, TCPAgent*>::iterator, bool> ret =
        m_TCPAgentMap.insert(map<string, TCPAgent*>::value_type(AgentType, pAgent));
    if(!ret.second)
    {
        DEV_LOG(LERROR, OUT_BOTH, "AgentManager: the AgentType has existed: " + AgentType);
        return FAILED;
    }
//    cout << "AgentManager::addAgent : " << pAgent << endl;
    return SUCCESSFUL;
}

int AgentManager::deleteAgent(const TCPAgent* pAgent, const string& AgentType)
{
    map<string, TCPAgent*>::iterator iter = m_TCPAgentMap.find(AgentType);
//"iter->second == pAgent" is needed,
//becuase if a Agent call addAgent fault(already had that type of agent in m_TCPAgentMap) and then call deleteAgent,
//we should verify the agent itself
    if(iter != m_TCPAgentMap.end() && iter->second == pAgent)
    {
        m_TCPAgentMap.erase(AgentType);
        DEV_LOG(LEVENT, OUT_BOTH, "AgentManager::deleteAgent  OK");
    }
    return SUCCESSFUL;
}

TCPAgent* AgentManager::getAgent(const string& AgentType)
{
    TCPAgent* pAgent = NULL;
    map<string, TCPAgent*>::iterator iter = m_TCPAgentMap.find(AgentType);
    if(iter != m_TCPAgentMap.end())
        pAgent = iter ->second;
    else
    {
        DEV_LOG(LERROR, OUT_BOTH, "can't find " + AgentType);
//        cout << "AgentType=" << AgentType << endl;
    }
    return pAgent;
}

int AgentManager::updateAddr(const string& type, const tr1::shared_ptr<SocketAddress>& addr)
{
    map<string, tr1::shared_ptr<SocketAddress> >::iterator iter = m_AddrMap.find(type);
    if(iter != m_AddrMap.end())
    {
        m_AddrMap.erase(iter);
    }
    pair<map<string, tr1::shared_ptr<SocketAddress> >::iterator, bool> ret =
        m_AddrMap.insert(map<string, tr1::shared_ptr<SocketAddress> >::value_type(type, addr));
    if(!ret.second)
    {
        DEV_LOG(LERROR, OUT_BOTH, "AgentManager::updateAddr error");
        return FAILED;
    }
    return SUCCESSFUL;
}

int AgentManager::getAddr(const string& type, SocketAddress& addr)
{
    map<string, tr1::shared_ptr<SocketAddress> >::iterator iter = m_AddrMap.find(type);
    if(iter != m_AddrMap.end())
    {
        addr = *(iter->second);
        return SUCCESSFUL;
    }
    else
        return FAILED;
}
