/*
 * @file MUAgentManager.cpp
 * @brief Agent manager of MU.
 *
 * @version 1.0
 * @date Wed Jun 27 21:36:10 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "MUAgentManager.h"

#include <inttypes.h>
#include <assert.h>

#include <map>

#include "log/log.h"
#include "comm/comm.h"

MUAgentManager::~MUAgentManager()
{

}

MUAgentManager::MUAgentManager()
{

}

bool
MUAgentManager::put(uint64_t agentId, MUTCPAgent *agent)
{
    std::pair<std::map<uint64_t, MUTCPAgent *>::iterator, bool> rc;

    rc = m_AgentMap.insert(std::pair<uint64_t, MUTCPAgent *>(agentId, agent));

    return rc.second;
}

MUTCPAgent *
MUAgentManager::get(uint64_t agentId)
{
    std::map<uint64_t, MUTCPAgent *>::iterator it = m_AgentMap.find(agentId);

    if (m_AgentMap.end() != it) {
        return it->second;
    }

    return NULL;
}

void
MUAgentManager::remove(uint64_t agentId)
{
    m_AgentMap.erase(agentId);
}

void
MUAgentManager::recycle(MUTCPAgent *agent)
{
    assert(agent);

    agent->recycler(true);

    uint64_t agentId = agent->getAgentId();
    remove(agentId);

    std::pair<std::map<uint64_t, MUTCPAgent *>::iterator, bool> ret;
    ret = m_RecycleMap.insert(std::pair<uint64_t, MUTCPAgent *>(agentId, agent));

    if (!ret.second) {
        TRACE_LOG("the same agent already exists in recycle list");
    }
}

void
MUAgentManager::doRecycle()
{
    TRACE_LOG("do agent recycle, %" PRIi32 " agents in recyle list",
              m_RecycleMap.size());

    int recycled = 0;

    for (std::map<uint64_t, MUTCPAgent *>::iterator it = m_RecycleMap.begin();
         it != m_RecycleMap.end();) {
        delete it->second;
        it->second = NULL;

        m_RecycleMap.erase(it++);
        ++recycled;
    }

    TRACE_LOG("%" PRIi32 " agents recycled", recycled);
}

