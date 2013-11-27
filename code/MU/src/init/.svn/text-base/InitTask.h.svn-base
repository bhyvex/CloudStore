/*
 * @file InitTask.h
 * @brief System init.
 * 
 * @version 1.0
 * @date Fri Jul 20 15:04:20 2012
 * 
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef INIT_INITTASK_H_
#define INIT_INITTASK_H_

#include <list>
#include <map>

#include "frame/MUTask.h"
#include "frame/MUTCPAgent.h"
#include "frame/MUWorkItem.h"


class InitTask : public MUTask
{
public:
    virtual ~InitTask();

    InitTask(MUTask *pParent);

    int next(MUTCPAgent *pAgent, const InReq &req);

    int next(MUWorkItem *pItem);

    int next(MUTask *pTask);

    void destroy(MUTCPAgent *pAgent);

    void destroy(MUTask *pTask);

    int start();

    int connectCallback(MUTCPAgent *pAgent, bool bConn);

private:
    int dispatch(MUWorkItem *pItem);

    int dispatchInitItem(MUWorkItem *pItem);

    int connectToRS();
    
    int getRule();

    int getRule(const InReq &req);

    int createBuckets();

    int createBuckets(MUWorkItem *pItem);

    int startBuckets();

    int startBuckets(MUTask *pTask);

    int startSystem();

private:
    enum _InitTaskItemType {
        INIT_ITEM,
    };

    enum _InitTaskState {
        TASK_INIT,
        TASK_GET_RULE,
        TASK_CREATE_BUCKETS,
        TASK_START_BUCKETS,
        TASK_START_SYSTEM,
        TASK_FINISH,
        TASK_ERROR,
    };

private:
    MUTCPAgent *m_pRSAgent;
    std::list<uint64_t> m_BucketList;
    std::map<uint64_t, MUTask *> m_StartBucketTaskMap;
};


#endif  // INIT_INITTASK_H_

