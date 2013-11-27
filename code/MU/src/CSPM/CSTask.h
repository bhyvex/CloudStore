/*
 * @file CSTask.h
 * @brief Process CS commands.
 *
 * @version 1.0
 * @date Wed Jul 11 11:29:44 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef CSPM_CSTask_H_
#define CSPM_CSTask_H_

#include "frame/PassiveTask.h"
#include "frame/MUTCPAgent.h"
#include "frame/MUWorkItem.h"

#include "MigrationClientTask.h"
#include "StartBucketTask.h"
#include "StopBucketTask.h"
#include "ExtentTask.h"

class CSTask : public PassiveTask
{
public:
    virtual ~CSTask();

    explicit CSTask(MUTask *pParent);

    void destroy(MUTCPAgent *pChannel);

    void destroy(MUTask *pChannel);

    int next(MUTCPAgent *pAgent, const InReq &req);

    int next(MUWorkItem *pItem);

    int next(MUTask *pTask);

private:
    /**
     * @brief Response to client when errors occurred.
     *
     * @param cmd Protocol command.
     */
    void errorResponse(uint32_t cmd);

    /**
     * @brief Simple response without extra data.
     *
     * @param cmd Protocol command.
     * @param error Error code.
     */
    void simpleResponse(uint32_t cmd, uint32_t error);

    int setBucketState(const InReq &req);

    int deleteBucket(const InReq &req);

    int migrateBucket(const InReq &req);

    int prepareExtentBucket(const InReq &req);

    int extentBucket(const InReq &req);

    void destroyExtentTask(MUTask *pTask);

    int completeExtent(MUTask *pTask);

    int dispatch(MUWorkItem *pItem);

    int dispatchBucketItem(MUWorkItem *pItem);

    int deleteBucket(MUWorkItem *pItem);

    int dispatch(MUTCPAgent *pAgent, const InReq &req);

    void destroyMigrationTask(MUTask *pTask);

    int completeMigration(MUTask *pTask);

    int startBucket(uint64_t bucketId);

    void destroyStartBucketTask(MUTask *pTask);

    int completeStartBucket(MUTask *pTask);

    int stopBucket(uint64_t bucketId);

    void destroyStopBucketTask(MUTask *pTask);

    int completeStopBucket(MUTask *pTask);

private:
    enum _CSTaskWorkItemType {
        BUCKET_ITEM,
    };

    enum _CSTaskState {
        TASK_INIT,
        TASK_WORKING,
        TASK_START_BUCKET,
        TASK_START_BUCKET_COMPLETED,
        TASK_STOP_BUCKET,
        TASK_STOP_BUCKET_COMPLETED,
        TASK_MIGRATION_STARTED,
        TASK_MIGRATION_COMPLETED,
        TASK_EXTENT_STARTED,
        //TASK_EXTENT_PREPARED,
        TASK_EXTENT_COMPLETED,
        TASK_FINISH,
        TASK_ERROR,
    };

private:
    MigrationClientTask *m_pMigrationTask;
    ExtentTask *m_pExtentTask;
    StartBucketTask *m_pStartBucketTask;
    StopBucketTask *m_pStopBucketTask;
};

#endif  // CSPM_CSTask_H_

