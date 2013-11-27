/*
 * @file StopBucketTask.h
 * @brief  stop a running bucket.
 *
 * @version 1.0
 * @date Sun Jul 15 17:41:30 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef CSPM_StopBucketTask_H_
#define CSPM_StopBucketTask_H_

#include "frame/MUTask.h"
#include "frame/MUTimer.h"

#include <inttypes.h>

class StopBucketTask : public MUTask
{
public:
    virtual ~StopBucketTask();

    StopBucketTask(MUTask *pParent);

    int start();

    inline void setBucketId(uint64_t bucketId);

    int next(MUTimer *pChannel, uint64_t times);

    void destroy(MUTimer *pChannel);

private:

    int registerTimer();

    int checkRequestQueue();

private:
    enum _StopBucketTaskState {
        TASK_INIT,
        TASK_CHECK_REQUEST_QUEUE,
        TASK_CHECK_REQUEST_QUEUE_COMPLETED,
        TASK_FINISH,
        TASK_ERROR,
    };

private:
    uint64_t m_BucketId;
    MUTimer *m_pTimer;
};

void
StopBucketTask::setBucketId(uint64_t bucketId)
{
    m_BucketId = bucketId;
}

#endif  // CSPM_StopBucketTask_H_

