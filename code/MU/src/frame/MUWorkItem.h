/*
 * @file MUWorkItem.h
 * @brief Thread pool work item in MU.
 * 
 * @version 1.0
 * @date Thu Jun 28 16:40:13 2012
 * 
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef _MUWORKITEM_H_
#define _MUWORKITEM_H_

#include <string>

#include <sys/types.h>

#include "sys/sys.h"

#include "MUTask.h"
#include "ReturnStatus.h"

class MUTPDispatcher;

class MUWorkItem : public ThreadPoolWorkItem3
{
public:
    virtual ~MUWorkItem();

    explicit MUWorkItem(MUTask *pTask);

    virtual int preProcess();

    virtual int process();

    virtual int postProcess();

    /**
     * @brief Send this work item to thread pool
     */
    void postRequest(int bucketID = -1);

    /**
     * @brief put this item into a map, then execute it in future
     */
    virtual void delay();

    /**
     * @brief Get Id of this item's owner task.
     *
     * @return Task Id.
     */
    uint64_t getTaskId() const;

    /**
     * @brief Get id of this item.
     *
     * @return Item id.
     */
    uint64_t getItemId() const;

    /**
     * @brief Get result value of processing.
     *
     * @return Result value.
     */
    const ReturnStatus& getReturnStatus() const;

    /**
     * @brief what type of work will be done.
     *
     * @param type of work
     */
    void setWorkType(int type);

    /**
     * @brief what type of work done by this work item.
     *
     * @return type of work
     */
    int getWorkType();

    /**
     * @brief real type of this item, 
     *          this attr will be used to identify the real type of this item
     *          object by the item's owner task
     *
     * @param type real type of this item
     */
    void setItemType(int type);

    /**
     * @brief Get real type of this object
     *
     * @return real type of this object
     */
    int getItemType();

    virtual uint64_t getBucketId();

    virtual void setBucketId(uint64_t bucketId);

protected:
    /**
     * @brief Send the result of processing back to its owner.
     */
    void postResult();

    /**
     * @brief Generate an unique id for this item object.
     */
    void generateItemId();

protected:
    uint64_t m_ItemId;  // unique id of this object
    uint64_t m_TaskId;  // owner of this work item
    int m_WorkType;  // type of work
    int m_ItemType;  // real type of this object
    ReturnStatus m_ReturnStatus;  // result value of processing

    // group items by bucket id
    uint64_t m_BucketId;
};

#endif  // _MUWORKITEM_H_

