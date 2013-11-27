/*
 * @file MUAgentManager.h
 * @brief Agent manager of MU.
 *
 * @version 1.0
 * @date Wed Jun 27 21:28:33 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef MUAgentManager_H_
#define MUAgentManager_H_

#include <inttypes.h>

#include <map>
#include <list>
#include <set>

#include "log/log.h"
#include "sys/sys.h"

#include "MUTCPAgent.h"

class MUAgentManager : public Singleton<MUAgentManager>
{
    friend class Singleton<MUAgentManager>;

public:
    virtual ~MUAgentManager();

    /**
     * @brief Put an agent into manager.
     *
     * @param agentId
     * @param agent
     *
     * @return true - success
     *         false - fail
     */
    bool put(uint64_t agentId, MUTCPAgent *agent);

    /**
     * @brief Get an agent by id.
     *
     * @param agentId
     *
     * @return NULL - no such agent
     *         agent - Agent identified by agentId
     */
    MUTCPAgent *get(uint64_t agentId);

    /**
     * @brief Remove an agent from manager.
     *
     * @param agentId
     */
    void remove(uint64_t agentId);

    // factory methods to create agent
    template <typename ConcreteAgent>
    ConcreteAgent *createAgent(Epoll *pEpoll);

    template <typename ConcreteAgent>
    ConcreteAgent *createAgent(const SocketAddress &addr, Epoll *pEpoll);

    template <typename ConcreteAgent>
    ConcreteAgent *createAgent(const TCPSocket &sock, const SocketAddress &addr, Epoll *pEpoll);

    /**
     * @brief Recycle an agent, put it into recyle list.
     *
     * @param agent
     */
    void recycle(MUTCPAgent *agent);

    /**
     * @brief delete agents in recycle list
     */
    void doRecycle();

protected:
    uint64_t generateId() {
        return Time::now(Time::Monotonic).toMicroSeconds();
    }

protected:
    // can't new
    MUAgentManager();

private:
    std::map<uint64_t, MUTCPAgent *> m_AgentMap;
    std::map<uint64_t, MUTCPAgent *> m_RecycleMap;
};

template <typename ConcreteAgent>
ConcreteAgent *MUAgentManager::createAgent(Epoll *epoll)
{
    ConcreteAgent *agent = new ConcreteAgent(epoll);

    uint64_t id = generateId();
    agent->setAgentId(id);

    while (!put(id, agent)) {
        WARN_LOG("Put agent %" PRIu64 " into MUAgentManager failed.", id);
    }

    return agent;
}

template <typename ConcreteAgent>
ConcreteAgent *MUAgentManager::createAgent(
    const SocketAddress &addr,
    Epoll *epoll)
{
    ConcreteAgent *agent = new ConcreteAgent(addr, epoll);

    uint64_t id = generateId();
    agent->setAgentId(id);

    while (!put(id, agent)) {
        WARN_LOG("Put agent %" PRIu64 " into MUAgentManager failed.", id);
    }

    return agent;
}

template <typename ConcreteAgent>
ConcreteAgent *MUAgentManager::createAgent(
    const TCPSocket &sock,
    const SocketAddress &addr,
    Epoll *epoll)
{
    ConcreteAgent *agent = new ConcreteAgent(sock, addr, epoll);

    uint64_t id = generateId();
    agent->setAgentId(id);

    while (!put(id, agent)) {
        WARN_LOG("Put agent %" PRIu64 " into MUAgentManager failed.", id);
    }

    return agent;
}

#endif  // MUAgentManager_H_

