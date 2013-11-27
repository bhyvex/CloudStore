/*
 * @file PassiveTask.h
 * @brief A passive task is created by MUTCPListenAgent
 *          and linked with a MUTCPAgent.
 *
 * @version 1.0
 * @date Fri Jul 13 15:46:29 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef frame_PassiveTask_H_
#define frame_PassiveTask_H_

#include "MUTask.h"
#include "MUTCPAgent.h"

class PassiveTask : public MUTask
{
public:
    virtual ~PassiveTask();

    PassiveTask(MUTask *pParent);

    inline void setOwner(MUTCPAgent *pAgent);

protected:
    MUTCPAgent *m_pOwner;
};

void
PassiveTask::setOwner(MUTCPAgent *pAgent)
{
    m_pOwner = pAgent;
}

#endif  // frame_PassiveTask_H_

