/*
 * @file MUEpoll.cpp
 * @brief
 *
 * @version 1.0
 * @date Mon Dec  3 15:55:23 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "MUEpoll.h"

#include "MUTaskManager.h"
#include "MUAgentManager.h"

int
MUEpoll::doTask()
{
    MUTaskManager::getInstance()->doRecycle();
    MUAgentManager::getInstance()->doRecycle();

    return 0;
}

