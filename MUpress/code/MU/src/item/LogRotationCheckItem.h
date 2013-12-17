/*
 * @file LogRotationCheckItem.h
 * @brief Check current status to decide whether log rotations should be done.
 *
 * @version 1.0
 * @date Tue Jul 10 11:13:09 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef _oplog_LogRotationCheckItem_H_
#define _oplog_LogRotationCheckItem_H_

#include "frame/MUWorkItem.h"
#include "frame/MUTask.h"

#include <list>
#include <map>

#include <inttypes.h>

class LogRotationCheckItem : public MUWorkItem
{
public:
    enum _LogRotationCheckItemWorkType {
        CHECK_LOG_FILE_SIZE
    };

public:
    virtual ~LogRotationCheckItem();

    LogRotationCheckItem(MUTask *pTask);

    inline void setBucketList(const std::list<uint64_t> &bucketList);

    inline const std::map<uint64_t, uint64_t> &getLogMap();

    int process();

private:
    void checkLogFileSize();

private:
    std::list<uint64_t> m_BucketList;
    std::map<uint64_t, uint64_t> m_LogMap;
};

void
LogRotationCheckItem::setBucketList(const std::list<uint64_t> &bucketList)
{
    m_BucketList = bucketList;
}

const std::map<uint64_t, uint64_t> &
LogRotationCheckItem::getLogMap()
{
    return m_LogMap;
}

#endif  // _oplog_LogRotationCheckItem_H_


