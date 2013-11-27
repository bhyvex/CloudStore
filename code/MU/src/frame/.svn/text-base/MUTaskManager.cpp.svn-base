/*
 * @file MUTaskManager.cpp
 * @brief Task manager of MU.
 *
 * @version 1.0
 * @date Wed Jun 27 21:36:10 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "MUTaskManager.h"

#include <inttypes.h>

#include <map>

#include "log/log.h"

#include "MUTask.h"

MUTaskManager::~MUTaskManager()
{
    /*
    for (std::map<uint64_t, MUTask *>::iterator it = m_TaskMap.begin();
            it != m_TaskMap.end(); ) {
        delete it->second;
        m_TaskMap.erase(it++);
    }
    */
}

MUTaskManager::MUTaskManager()
{

}

bool
MUTaskManager::put(uint64_t taskId, MUTask *pTask)
{
    std::pair<std::map<uint64_t, MUTask *>::iterator, bool> rc;

    rc = m_TaskMap.insert(std::pair<uint64_t, MUTask *>(taskId, pTask));

    return rc.second;
}

MUTask *
MUTaskManager::get(uint64_t taskId)
{
    std::map<uint64_t, MUTask *>::iterator it = m_TaskMap.find(taskId);

    if (m_TaskMap.end() != it) {
        return it->second;
    }

    // look up in recycle list
    it = m_RecycleMap.find(taskId);

    if (m_RecycleMap.end() != it
        && it->second->getRefCnt() != 0) {
        return it->second;
    }

    return NULL;
}

void
MUTaskManager::remove(uint64_t taskId)
{
    m_TaskMap.erase(taskId);
}

void
MUTaskManager::recycle(MUTask *pTask)
{
    uint64_t taskId = pTask->getTaskId();

    remove(taskId);

    std::pair<std::map<uint64_t, MUTask *>::iterator, bool> ret;
    ret = m_RecycleMap.insert(std::pair<uint64_t, MUTask *>(taskId, pTask));

    if (!ret.second) {
        TRACE_LOG("the same task already exists in recycle list");
    }
}

void
MUTaskManager::doRecycle()
{
    TRACE_LOG("do task recycle, %" PRIi32 " tasks in recyle list",
              m_RecycleMap.size());

    int recycled = 0;

    for (std::map<uint64_t, MUTask *>::iterator it = m_RecycleMap.begin();
         it != m_RecycleMap.end();) {
        if (it->second->getRefCnt() == 0) {
            delete it->second;
            it->second = NULL;

            m_RecycleMap.erase(it++);
            ++recycled;

            continue;
        }

        ++it;
    }

    TRACE_LOG("%" PRIi32 " tasks recycled", recycled);
}


