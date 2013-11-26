/*
 * @file MigrationServerTask.h
 * @brief Server task for data migration.
 *
 * @version 1.0
 * @date Mon Jul 16 01:06:31 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef MigrationPM_MigrationServerTask_H_
#define MigrationPM_MigraitonServerTask_H_

#include "frame/PassiveTask.h"
#include "frame/MUTCPAgent.h"
#include "frame/MUWorkItem.h"
#include "item/MigrationServerItem.h"
#include "CSPM/StopBucketTask.h"
#include "CSPM/StartBucketTask.h"

class MigrationServerTask : public PassiveTask
{
public:
    virtual ~MigrationServerTask();

    MigrationServerTask(MUTask *pParent);

    void destroy(MUTCPAgent *pChannel);

    void destroy(MUTask *pTask);

    int next(MUTCPAgent *pAgent, const InReq &req);

    int next(MUWorkItem *pItem);

    int next(MUTask *pTask);

private:
    int dispatchMigrationServerItem(MUWorkItem *pItem);

    void destroyStopBucketTask(MUTask *pTask);

    void destroyStartBucketTask(MUTask *pTask);

    int handshake(const InReq &req);

    int stopBucket();

    int stopBucket(MUTask *pTask);

    int tarBucket();

    int tarBucket(MUWorkItem *pItem);

    int migrate(const InReq &req);

    int migrate(MUWorkItem *pItem);

    int deleteBucket(const InReq &req);

    int deleteBucket(MUWorkItem *pItem);

    int startBucket();

    int startBucket(MUTask *pTask);

    void recovery();

private:
    enum _MigrationServerTaskState {
        TASK_INIT,
        TASK_STOP_BUCKET,
        TASK_STOP_BUCKET_COMPLETED,
        TASK_TAR_BUCKET,
        TASK_TAR_BUCKET_COMPLETED,
        TASK_MIGRATE_DATA,
        TASK_MIGRATE_DATA_COMPLETED,
        TASK_DELETE_BUCKET,
        TASK_DELETE_BUCKET_COMPLETED,
        TASK_FINISH,
        TASK_ERROR,
        TASK_RECOVERY,
        TASK_RECOVERY_COMPLETED,
    };

    enum _MigrationServerTaskItemType {
        MIGRATION_SERVER_ITEM,
        BUCKET_ITEM,
    };

private:
    uint64_t m_BucketId;
    StopBucketTask *m_pStopBucketTask;
    StartBucketTask *m_pStartBucketTask;
    MigrationServerItem *m_pItem;
};


#endif  // MigrationPM_MigrationServerTask_H_

