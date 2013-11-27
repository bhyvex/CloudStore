/*
 * @file ExtentTask.h
 * @brief for bucket extent
 *
 * @version 1.0
 * @date Mon Jul 16 09:52:20 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef CSPM_ExtentTask_H_
#define CSPM_ExtentTask_H_

#include <list>

#include "frame/MUTask.h"
#include "frame/MUTimer.h"
#include "frame/MUWorkItem.h"

class ExtentTask : public MUTask
{
public:
    virtual ~ExtentTask();

    ExtentTask(MUTask *pParent);

    void destroy(MUTimer *pChannel);

    int start();

    int next(MUTimer *pTimer, uint64_t times);

    int next(MUWorkItem *pItem);

    inline void setModNr(uint64_t modNr);

private:

    int dispatch(MUWorkItem *pItem);

    int dispatchExtentItem(MUWorkItem *pItem);

    int checkRequestQueue();

    int noRequests();

    int extent();

    int extent(MUWorkItem *pItem);

    void openBigLock();

    bool inBucketList(
        uint64_t bucketId,
        const std::list<uint64_t> &bucketList);

private:
    enum _ExtentTaskItemType {
        EXTENT_ITEM,
    };

    enum _ExtentTaskState {
        TASK_INIT,
        TASK_PREPARE,
        TASK_PREPARE_COMPLETED,
        TASK_EXTENT,
        TASK_EXTENT_COMPLETED,
        TASK_FINISH,
        TASK_ERROR,
    };

private:
    uint64_t m_ModNr;
    MUTimer *m_pTimer;
    std::list<uint64_t> m_BucketList;
    bool m_Failed;
};

void
ExtentTask::setModNr(uint64_t modNr)
{
    m_ModNr = modNr;
}

#endif  // CSPM_ExtentTask_H_

