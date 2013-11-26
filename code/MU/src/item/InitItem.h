/*
 * @file InitItem.h
 * @brief Init system.
 *
 * @version 1.0
 * @date Fri Jul 20 15:08:54 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef item_InitItem_H_
#define item_InitItem_H_

#include <list>

#include "frame/MUWorkItem.h"
#include "frame/MUTask.h"

class InitItem : public MUWorkItem
{
public:
    enum _InitItemWorkType {
        CREATE_BUCKETS,
    };

public:
    virtual ~InitItem();

    explicit InitItem(MUTask *pTask);

    inline void setBucketList(const std::list<uint64_t> &bucketList);

    int process();

private:
    void createBuckets();

private:
    std::list<uint64_t> m_BucketList;
};

void
InitItem::setBucketList(const std::list<uint64_t> &bucketList)
{
    m_BucketList = bucketList;
}

#endif  // item_InitItem_H_


