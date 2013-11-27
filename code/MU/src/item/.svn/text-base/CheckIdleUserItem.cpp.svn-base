/*
 * @file CheckIdleUserItem.cpp
 * @brief  check and delete outdated user.
 *
 * @version 1.0
 * @date Tue Aug  7 16:35:42 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "CheckIdleUserItem.h"

#include "log/log.h"

#include "state/UserManager.h"

CheckIdleUserItem::~CheckIdleUserItem()
{

}

CheckIdleUserItem::CheckIdleUserItem(MUTask *pTask) :
    MUWorkItem(pTask)
{

}

int
CheckIdleUserItem::process()
{
    switch (m_WorkType) {

    case CHECK_IDLE_USER: {
            checkIdleUser();
            break;
        }

    default: {
            DEBUG_LOG("unexpected work type %d", m_WorkType);
            m_ReturnStatus = ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
            break;
        }

    }

    return 0;
}

void
CheckIdleUserItem::checkIdleUser()
{
    UserManager::getInstance()->checkIdleUsers();

    m_ReturnStatus = ReturnStatus(MU_SUCCESS);
}

