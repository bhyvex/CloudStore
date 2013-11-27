/*
 * @file CheckIdleUserTask.h
 * @brief  stop a running bucket.
 *
 * @version 1.0
 * @date Sun Jul 15 17:41:30 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef frame_CheckIdleUserTask_H_
#define frame_CheckIdleUserTask_H_

#include "frame/MUTask.h"
#include "frame/MUTimer.h"
#include "frame/MUWorkItem.h"

#include <inttypes.h>

class CheckIdleUserTask : public MUTask
{
public:
    virtual ~CheckIdleUserTask();

    explicit CheckIdleUserTask(MUTask *pParent);

    int start();

    int next(MUTimer *pChannel, uint64_t times);

    int next(MUWorkItem *pItem);

    void destroy(MUTimer *pChannel);

private:
    int registerTimer();

    int checkIdleUser();

    int dispatch(MUWorkItem *pItem);

    int checkIdleUser(MUWorkItem *pItem);

private:
    enum _CheckIdleUserTaskState {
        TASK_INIT,
        TASK_CHECK_IDLE_USER,
        TASK_FINISH,
        TASK_ERROR,
    };

    enum _CheckIdleUserTaskItemType {
        CHECK_IDLE_USER_ITEM,
    };

private:
    MUTimer *m_pTimer;
};

#endif  // frame_CheckIdleUserTask_H_

