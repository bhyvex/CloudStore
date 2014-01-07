/*
 * @file MUWorkItem.cpp
 * @brief Thread pool work item in MU.
 *
 * @version 1.0
 * @date Thu Jun 28 16:40:42 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "MUWorkItem.h"

#include <string>
#include <assert.h>

#include "MURegister.h"
#include "MUWorkItemManager.h"
#include "MUTPDispatcher.h"
#include "ReturnStatus.h"

#include "log/log.h"
#include "sys/sys.h"
#include "storage/ChannelManager.h"
#include "storage/Channel.h"


#include <limits.h>

MUWorkItem::~MUWorkItem()
{

}

MUWorkItem::MUWorkItem(MUTask *pTask)
{
    assert(pTask);

    m_BucketId = UINT64_MAX;
    m_WorkType = 0;
    m_ItemType = 0;

    m_TaskId = pTask->getTaskId();

    generateItemId();
}

int
MUWorkItem::preProcess()
{
    return 0;
}

int
MUWorkItem::process()
{
    return 0;
}

int
MUWorkItem::postProcess()
{
    postResult();
    return 0;
}

void
MUWorkItem::postResult()
{
    MUTPDispatcher *pDispatcher = NULL;
    pDispatcher = MURegister::getInstance()->getThreadPoolDispatcher();

    assert(pDispatcher);

    pDispatcher->postResult(this);
}

uint64_t
MUWorkItem::getTaskId() const
{
    return m_TaskId;
}

uint64_t
MUWorkItem::getItemId() const
{
    return m_ItemId;
}

const ReturnStatus &
MUWorkItem::getReturnStatus() const
{
    return m_ReturnStatus;
}

void
MUWorkItem::setWorkType(int type)
{
    m_WorkType = type;
}

int
MUWorkItem::getWorkType()
{
    return m_WorkType;
}

void
MUWorkItem::setItemType(int type)
{
    m_ItemType = type;
}

int
MUWorkItem::getItemType()
{
    return m_ItemType;
}

void
MUWorkItem::generateItemId()
{
    m_ItemId = Time::now(Time::Monotonic).toMicroSeconds();
}

void
MUWorkItem::postRequest(int bucketID)
{
	/*
    MUWorkItemManager *pItemManager =
        MURegister::getInstance()->getCurrentItemManager();
    pItemManager->put(m_ItemId, this);

    MUTPDispatcher *pDispatcher = NULL;
    pDispatcher = MURegister::getInstance()->getThreadPoolDispatcher();

    assert(pDispatcher);

    pDispatcher->postRequest(this);
    */

    
    ThreadPool3 *pThreadPool3 = MURegister::getInstance()->getThreadPool();

    if(bucketID == -1){
    	pThreadPool3->postRequest(this);
    }else{
    	Channel* pDataChannel = ChannelManager::getInstance()->Mapping(bucketID);
    	int threadID = pDataChannel->m_Id;
    	pThreadPool3->postRequest(threadID, this);
    }
    
}


void
MUWorkItem::delay()
{
    MUWorkItemManager *pItemManager =
        MURegister::getInstance()->getDelayItemManager();
    pItemManager->put(m_ItemId, this);
}

uint64_t
MUWorkItem::getBucketId()
{
    return m_BucketId;
}

void
MUWorkItem::setBucketId(uint64_t bucketId)
{
    m_BucketId = bucketId;
}


