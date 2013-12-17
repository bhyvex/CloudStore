/*
 * @file PassiveTask.cpp
 * @brief A passive task is created by MUTCPListenAgent
 *          and linked with a MUTCPAgent.
 * 
 * @version 1.0
 * @date Fri Jul 13 16:07:52 2012
 * 
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "PassiveTask.h"

PassiveTask::~PassiveTask()
{
    if (NULL != m_pOwner) {
        delete m_pOwner;
        m_pOwner = NULL;
    }
}

PassiveTask::PassiveTask(MUTask *pParent) :
    MUTask(pParent)
{
    m_pOwner = NULL;
}

