/*
 * @file MigrationClientTask.h
 * @brief data migration client.
 *
 * @version 1.0
 * @date Wed Jul 11 17:42:45 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef CSPM_MigrationClientTask_H_
#define CSPM_MigrationClientTask_H_

#include <inttypes.h>

#include <string>

#include "frame/MUTask.h"
#include "frame/MUTCPAgent.h"
#include "item/MigrationClientItem.h"
#include "StartBucketTask.h"

class MigrationClientTask : public MUTask
{
public:
    virtual ~MigrationClientTask();

    MigrationClientTask(MUTask *pParent);

    inline void setBucketId(uint64_t bucketId);

    inline uint64_t getBucketId();

    inline void setSrcMUIP(const std::string &ip);

    inline std::string getSrcMUIP();

    void destroy(MUTCPAgent *pChannel);

    void destroy(MUTask *pTask);

    int next(MUTCPAgent *pAgent, const InReq &req);

    int next(MUWorkItem *pItem);

    int next(MUTask *pTask);

    virtual int connectCallback(MUTCPAgent *pAgent, bool bConn);

    int start();

private:
    int dispatchMigrationClientItem(MUWorkItem *pItem);

    int handshake();

    int handshake(MUTCPAgent *pAgent, const InReq &req);

    int migrate();

    int migrate(MUTCPAgent *pAgent, const InReq &req);

    int flush(const InReq &req);

    int flush(MUWorkItem *pItem);

    int extract();

    int extract(MUWorkItem *pItem);

    int startBucket();

    int startBucket(MUTask *pTask);

    int deleteBucket();

    int deleteBucket(const InReq &req);

private:
    enum _MigrationClientTaskState {
        TASK_INIT,
        TASK_HANDSHAKE,
        TASK_HANDSHAKE_COMPLETED,
        TASK_MIGRATE_DATA,
        TASK_MIGRATE_DATA_COMPLETED,
        TASK_EXTRACT_DATA,
        TASK_EXTRACT_DATA_COMPLETED,
        TASK_START_BUCKET,
        TASK_START_BUCKET_COMPLETED,
        TASK_DELETE_SRC_BUCKET,
        TASK_DELETE_SRC_BUCKET_COMPLETED,
        TASK_FINISH,
        TASK_ERROR,
    };

    enum _MigrationClientTaskItemType {
        MIGRATION_CLIENT_ITEM,
    };

private:
    MUTCPAgent *m_pAgent;

    uint64_t m_BucketId;
    std::string m_SrcMUIP;

    MigrationClientItem *m_pItem;
    bool m_MoreData;

    StartBucketTask *m_pStartBucketTask;
};

void
MigrationClientTask::setBucketId(uint64_t bucketId)
{
    m_BucketId = bucketId;
}

uint64_t
MigrationClientTask::getBucketId()
{
    return m_BucketId;
}

void
MigrationClientTask::setSrcMUIP(const std::string &ip)
{
    m_SrcMUIP = ip;
}

std::string
MigrationClientTask::getSrcMUIP()
{
    return m_SrcMUIP;
}

#endif  // CSPM_MigrationClientTask_H_


