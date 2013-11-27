/*
 * @file StartBucketTask.h
 * @brief start a bucket
 *
 * @version 1.0
 * @date Sun Jul 15 16:48:00 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef CSPM_StartBucketTask_H_
#define CSPM_StartBucketTask_H_

#include "frame/MUTask.h"
#include "frame/MUWorkItem.h"

#include <inttypes.h>

class StartBucketTask : public MUTask
{
public:
    virtual ~StartBucketTask();

    StartBucketTask(MUTask *pParent);

    inline void setBucketId(uint64_t bucketId);

    inline uint64_t getBucketId();

    int start();

    int next(MUWorkItem *pItem);

private:
    int startBucket();

    int startBucket(MUWorkItem *pItem);

    int dispatchBucketItem(MUWorkItem *pItem);

private:
    enum _StartBucketTaskItemType {
        BUCKET_ITEM,
    };

    enum _StartBucketTaskState {
        TASK_INIT,
        TASK_READ_BUCKET_INFO,
        TASK_READ_BUCKET_INFO_COMPLETED,
        TASK_FINISH,
        TASK_ERROR,
    };

private:
    uint64_t m_BucketId;
};

void
StartBucketTask::setBucketId(uint64_t bucketId)
{
    m_BucketId = bucketId;
}

uint64_t
StartBucketTask::getBucketId()
{
    return m_BucketId;
}

#endif  // CSPM_StartBucketTask_H_

