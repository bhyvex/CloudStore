/*
 * @file MUTask.cpp
 * @brief Base Task definition of MU module.
 *
 * @version 1.0
 * @date Wed Jun 27 21:22:47 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "MUTask.h"

#include "log/log.h"
#include "sys/sys.h"

#include "MUTaskManager.h"
#include "MUTCPAgent.h"
#include "MUTimer.h"
#include "MURegister.h"
#include "MUAgentManager.h"

#include <assert.h>

MUTask::~MUTask()
{
    if (NULL != m_pParent) {
        m_pParent->destroy(this);
        m_pParent = NULL;
    }
}

MUTask::MUTask(MUTask *pTask)
{
    m_CurrentState = 0;
    m_RefCnt = 0;

    m_pParent = pTask;

    generateTaskId();
}

uint64_t
MUTask::getTaskId()
{
    return m_TaskId;
}

void
MUTask::destroy(MUTCPAgent *pChannel)
{

}

void
MUTask::destroy(MUTask *pChannel)
{

}

void
MUTask::destroy(MUTimer *pChannel)
{

}

void
MUTask::generateTaskId()
{
    m_TaskId = Time::now(Time::Monotonic).toMicroSeconds();
}

int
MUTask::next(MUTCPAgent *pAgent, const InReq &req)
{
    return 0;
}

int
MUTask::next(MUWorkItem *pItem)
{
    return 0;
}


int
MUTask::next(MUTimer *pTimer, uint64_t times)
{
    return 0;
}

int
MUTask::connectCallback(MUTCPAgent *pAgent, bool bConn)
{
    return 0;
}

int
MUTask::next(MUTask *pTask)
{
    return 0;
}

void
MUTask::sendSimplePacket(MUTCPAgent *pAgent, uint32_t cmd, uint32_t error)
{
    MsgHeader msg;
    msg.cmd = cmd;
    msg.error = error;
    msg.length = 0;

    pAgent->sendPacket(msg, NULL);
}

void
MUTask::setParent(MUTask *pTask)
{
    m_pParent = pTask;
}

void
MUTask::sendItem(MUWorkItem *pItem, int bucketID)
{
/*
    if (MURegister::getInstance()->bigLock()) {
        pItem->delay();

    } else {
        pItem->postRequest();
    }
*/
	pItem->postRequest(bucketID);

    ref();
}

bool
MUTask::error()
{
    return false;
}

void
MUTask::recycle()
{
    if (NULL != m_pParent) {
        m_pParent->destroy(this);
        m_pParent = NULL;
    }

    MUTaskManager::getInstance()->recycle(this);
}

void
MUTask::recycle(MUTask *pTask)
{
    if (NULL != pTask) {
        pTask->setParent(NULL);
        MUTaskManager::getInstance()->recycle(pTask);
    }
}

void
MUTask::recycle(MUTCPAgent *pAgent)
{
    if (NULL != pAgent) {
        pAgent->setTask(NULL);
        MUAgentManager::getInstance()->recycle(pAgent);
    }
}

void
MUTask::recycle(MUTimer *pTimer)
{
    if (NULL != pTimer) {
        pTimer->setTask(NULL);
        delete pTimer;
    }
}

void
MUTask::ref()
{
    ++m_RefCnt;
}

void
MUTask::release()
{
    --m_RefCnt;
}

int
MUTask::getRefCnt()
{
    return m_RefCnt;
}

void
MUTask::closeAgent(MUTCPAgent *pAgent)
{
    pAgent->setTask(NULL);
    pAgent->close();
}


