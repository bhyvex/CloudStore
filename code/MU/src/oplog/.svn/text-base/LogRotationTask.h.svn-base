/*
 * @file LogRotationTask.h
 * @brief Task to do operation log rotation.
 *
 * @version 1.0
 * @date Tue Jul 10 09:47:50 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef _oplog_LogRotationTask_H_
#define _oplog_LogRotationTask_H_

#include "frame/MUTask.h"
#include "frame/MUTimer.h"

class LogRotationTask : public MUTask
{
public:
    virtual ~LogRotationTask();

    LogRotationTask(MUTask *pTask);

    virtual void destroy(MUTimer *pChannel);

    int next(MUTimer *pTimer, uint64_t times);

    int next(MUWorkItem *pItem);

    int start();

private:
    enum _WorkItemType {
        LOG_ROTATION_CHECK_ITEM
    };

private:
    int doRotationByTime();

    int doRotationBySize();

    int dispatch(MUWorkItem *pItem);

    int dispatchLogRotationCheckItem(MUWorkItem *pItem);

    int checkLogFileSize(MUWorkItem *pItem);

private:
    MUTimer *m_pTimer;
    std::string m_RotateStrategy;
    uint64_t m_LastRotateTime;
    uint64_t m_RotateTime;
    uint64_t m_RotateSize;
};

#endif  // _oplog_LogRotationTask_H_

