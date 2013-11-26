/*
 * @file MUWorkItemManager.h
 * @brief Thread pool work item container.
 *
 * @version 1.0
 * @date Fri Jun 29 11:16:12 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef MUWorkItemManager_H_
#define MUWorkItemManager_H_

#include <map>
#include <list>

#include <inttypes.h>

class MUWorkItem;

class MUWorkItemManager
{
public:
    virtual ~MUWorkItemManager();

    MUWorkItemManager();

    bool put(uint64_t itemId, MUWorkItem *pItem);

    MUWorkItem *get(uint64_t itemId);

    void remove(uint64_t itemId);

    bool noItemsInBucket(uint64_t bucketId);

    bool noItems();

    void getAllItems(std::list<uint64_t> *pItemList);

private:
    // work items that currently being executed, the key is their Id
    std::map<uint64_t, MUWorkItem *> m_ItemMap;
    std::multimap<uint64_t, MUWorkItem *> m_ItemMapByBucket;
};

#endif  // MUWorkItemManager_H_

