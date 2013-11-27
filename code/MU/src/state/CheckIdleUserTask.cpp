/*
 * @file CheckIdleUserTask.cpp
 * @brief  stop a running bucket.
 *
 * @version 1.0
 * @date Sun Jul 15 18:27:33 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "CheckIdleUserTask.h"

#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>

#include <memory>

#include "frame/MUTask.h"
#include "frame/MUTimer.h"
#include "frame/MUMacros.h"
#include "frame/MUTaskManager.h"
#include "frame/MURegister.h"
#include "item/CheckIdleUserItem.h"

#include "log/log.h"

CheckIdleUserTask::~CheckIdleUserTask()
{
    if (NULL != m_pTimer) {
        m_pTimer->setTask(NULL);
        delete m_pTimer;
        m_pTimer = NULL;
    }
}

CheckIdleUserTask::CheckIdleUserTask(MUTask *pParent) :
    MUTask(pParent)
{
    m_CurrentState = TASK_INIT;
    m_pTimer = NULL;
}

void
CheckIdleUserTask::destroy(MUTimer *pChannel)
{
    m_pTimer = NULL;

    FATAL_LOG("Check idle user task, timer error");
}

int
CheckIdleUserTask::start()
{
    switch (m_CurrentState) {
    case TASK_INIT: {
            return registerTimer();
            break;
        }

    default: {
            // never going here
            DEBUG_LOG("unexpected state %d", m_CurrentState);
            assert(0);
            break;
        }

    }

    return 0;
}

int
CheckIdleUserTask::next(MUTimer *pChannel, uint64_t times)
{
    int rt = 0;

    switch (m_CurrentState) {

    case TASK_CHECK_IDLE_USER: {
            rt = checkIdleUser();

            if (-1 == rt) {
                DEBUG_LOG("check idle user failed");
            }

            break;
        }

    default: {
            // never going here
            DEBUG_LOG("unexpected state %d.", m_CurrentState);
            assert(0);
            break;
        }

    }

    return 0;
}

int
CheckIdleUserTask::registerTimer()
{
    int rt = 0;

    m_pTimer = new MUTimer(MURegister::getInstance()->getEpoll());

    rt = m_pTimer->create();

    if (-1 == rt) {
        DEBUG_LOG("create timer failed");

        m_pTimer->setTask(NULL);
        delete m_pTimer;
        m_pTimer = NULL;

        return -1;
    }

    m_pTimer->setTask(this);
    m_pTimer->setTime(
        USER_IDLE_CHECK_PERIOD,
        USER_IDLE_CHECK_PERIOD
    );

    m_CurrentState = TASK_CHECK_IDLE_USER;

    return 0;
}

int
CheckIdleUserTask::checkIdleUser()
{
    CheckIdleUserItem *pItem = new CheckIdleUserItem(this);

    pItem->setItemType(CHECK_IDLE_USER_ITEM);
    pItem->setWorkType(CheckIdleUserItem::CHECK_IDLE_USER);

    pItem->postRequest();

    return 0;
}

int
CheckIdleUserTask::next(MUWorkItem *pItem)
{
    int rt = 0;

    rt = dispatch(pItem);

    if (-1 == rt) {
        DEBUG_LOG("dispatch item failed");
    }

    return 0;
}

int
CheckIdleUserTask::dispatch(MUWorkItem *pItem)
{
    int rt = 0;

    switch (pItem->getItemType()) {

    case CHECK_IDLE_USER_ITEM: {
            rt = checkIdleUser(pItem);

            if (-1 == rt) {
                DEBUG_LOG("check idle user failed");
            }

            break;
        }

    default: {
            DEBUG_LOG("unexpected item type %d", pItem->getItemType());
            return -1;
        }

    }

    return 0;
}

int
CheckIdleUserTask::checkIdleUser(MUWorkItem *pItem)
{
    std::auto_ptr<CheckIdleUserItem> pRItem =
        std::auto_ptr<CheckIdleUserItem>(
            dynamic_cast<CheckIdleUserItem *>(pItem));

    ReturnStatus rs = pItem->getReturnStatus();

    if (!rs.success()) {
        DEBUG_LOG("check idle user failed");
    }

    return 0;
}




