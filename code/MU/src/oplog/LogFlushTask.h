/*
 * @file LogFlushTask.h
 * @brief Task to flush logs to local disk.
 *
 * @version 1.0
 * @date Tue Jul 10 09:47:50 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef _oplog_LogFlushTask_H_
#define _oplog_LogFlushTask_H_

#include "frame/MUTask.h"
#include "frame/MUTimer.h"

class LogFlushTask : public MUTask
{
public:
    virtual ~LogFlushTask();

    LogFlushTask(MUTask *pTask);

    virtual void destroy(MUTimer *pChannel);

    int next(MUTimer *pTimer, uint64_t times);

    int start();

private:
    int flushLog();

private:
    MUTimer *m_pTimer;
};

#endif  // _oplog_LogFlushTask_H_

