/*
 * @file RecycleTask.h
 * @brief  stop a running bucket.
 *
 * @version 1.0
 * @date Sun Jul 15 17:41:30 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef frame_RecycleTask_H_
#define frame_RecycleTask_H_

#include "frame/MUTask.h"
#include "frame/MUTimer.h"

#include <inttypes.h>

class RecycleTask : public MUTask
{
public:
    virtual ~RecycleTask();

    RecycleTask(MUTask *pParent);

    int start();

    int next(MUTimer *pChannel, uint64_t times);

    void destroy(MUTimer *pChannel);

private:
    int registerTimer();

    void doRecycle();

private:
    enum _RecycleTaskState {
        TASK_INIT,
        TASK_RECYCLE,
        TASK_FINISH,
        TASK_ERROR,
    };

private:
    MUTimer *m_pTimer;
};

#endif  // frame_RecycleTask_H_

