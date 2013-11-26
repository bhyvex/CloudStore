/*
 * @file MigrationClientItem.h
 * @brief Async work for migration client.
 *
 * @version 1.0
 * @date Sun Jul 15 12:33:56 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef item_MigrationClientItem_H_
#define item_MigrationClientItem_H_

#include "frame/MUWorkItem.h"
#include "frame/MUTask.h"

#include <string>

#include <inttypes.h>

class MigrationClientItem : public MUWorkItem
{
public:
    enum _MigrationClientItemWorkType {
        WRITE_BUCKET_DATA,
        EXTRACT_BUCKET_DATA
    };

public:
    virtual ~MigrationClientItem();

    explicit MigrationClientItem(MUTask *pTask);

    int process();

    inline void setBucketData(const std::string &data);

    inline void setBucketId(uint64_t bucketId);

private:
    void writeBucketData();

    void extractBucketData();

private:
    int m_BucketId;
    int m_BucketDataFd;
    std::string m_BucketData;
};

void
MigrationClientItem::setBucketData(const std::string &data)
{
    m_BucketData = data;
}

void
MigrationClientItem::setBucketId(uint64_t bucketId)
{
    m_BucketId = bucketId;
}

#endif  // item_MigrationClientItem_H_

