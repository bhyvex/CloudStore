/*
 * @file BucketItem.h
 * @brief Bucket-related thread pool operations.
 *
 * @version 1.0
 * @date Wed Jul 11 15:26:09 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef item_BucketItem_H_
#define item_BucketItem_H_

#include "frame/MUWorkItem.h"
#include "frame/MUTask.h"

#include <inttypes.h>

class BucketItem : public MUWorkItem
{
public:
    enum _BucketItemWorkType {
        DELETE_BUCKET,
        START_BUCKET,
    };

public:
    virtual ~BucketItem();

    BucketItem(MUTask *pTask);

    int process();

    inline uint64_t getBucketId();

    inline void setBucketId(uint64_t bucketId);

    inline uint64_t getLogSeqNr();

private:
    void deleteBucket();

    void startBucket();

private:
    uint64_t m_BucketId;

    // for start bucket
    uint64_t m_LogSeqNr;

};


uint64_t 
BucketItem::getBucketId()
{
    return m_BucketId;
}

void
BucketItem::setBucketId(uint64_t bucketId)
{
    m_BucketId = bucketId;
}

uint64_t
BucketItem::getLogSeqNr()
{
    return m_LogSeqNr;
}

#endif  // item_BucketItem_H_

