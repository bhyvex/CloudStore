/*
 * @file MUTPDispatcher.cpp
 * @brief Request dispatcher of MU thread pool.
 *
 * @version 1.0
 * @date Thu Jun 28 15:43:39 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "MUTPDispatcher.h"

#include <unistd.h>

#include "MUWorkItem.h"
#include "MUTask.h"
#include "MUTaskManager.h"
#include "MUWorkItemManager.h"
#include "MURegister.h"

MUTPDispatcher::MUTPDispatcher(Epoll *pEpoll, ThreadPool *pPool) :
    ThreadPoolDispatcher(pEpoll, pPool)
{

}

int
MUTPDispatcher::recvData()
{
    MUWorkItem *pItem = NULL;

    MUTask *pTask = NULL;

    MUWorkItemManager *pItemManager = NULL;

    int rt = 0;

    for (; ;) {
        rt = ::read(m_hReadHandle, &pItem, sizeof(pItem));

        if (rt < 0) {
            if (EINTR == errno) {
                continue;

            } else if (EAGAIN == errno) {
                break;

            } else {
                DEBUG_LOG("Syscall Error: read. %s.", strerror(errno));
                break;
            }
        }

        pItemManager = MURegister::getInstance()->getCurrentItemManager();
        pItemManager->remove(pItem->getItemId());

        pTask = MUTaskManager::getInstance()->get(pItem->getTaskId());

        if (NULL == pTask) {  // Task已不存在
            DEBUG_LOG("Can't find task %llu.", pItem->getTaskId());

            delete pItem;
            pItem = NULL;

        } else {  // Task正常
            pTask->next(pItem);
        }
    }

    return 0;
}

int
MUTPDispatcher::postResult(MUWorkItem *pWorkItem)
{
    int rt = util::io::writen(m_hWriteHandle, &pWorkItem, sizeof(void *));

    if (sizeof(void *) != rt) {
        DEBUG_LOG("writen error");
    }

    return 0;
}

void
MUTPDispatcher::
postRequest(MUWorkItem *pWorkItem)
{
    ThreadPoolDispatcher::postRequest(pWorkItem);
}

