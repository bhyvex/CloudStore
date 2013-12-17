/*
 * @file MigrationServerItem.h
 * @brief Async work for migration server.
 *
 * @version 1.0
 * @date Mon Jul 16 01:30:46 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef item_MigrationServerItem_H_
#define item_MigrationServerItem_H_

#include "frame/MUWorkItem.h"
#include "frame/MUTask.h"

#include <string>

#include <inttypes.h>

class MigrationServerItem : public MUWorkItem
{
public:
    enum _MigrationServerItemWorkType {
        TAR_BUCKET_DATA,
        READ_BUCKET_DATA,
    };

public:
    virtual ~MigrationServerItem();

    explicit MigrationServerItem(MUTask *pTask);

    int process();

    inline const std::string &getBucketData();

    inline bool moreData();

    inline void setBucketId(uint64_t bucketId);

private:
    void readBucketData();

    void tarBucketData();

private:
    int m_BucketDataFd;
    bool m_MoreData;
    std::string m_BucketData;
};

const std::string &
MigrationServerItem::getBucketData()
{
    return m_BucketData;
}

void
MigrationServerItem::setBucketId(uint64_t bucketId)
{
    m_BucketId = bucketId;
}

bool
MigrationServerItem::moreData()
{
    return m_MoreData;
}

#endif  // item_MigrationServerItem_H_

