/*
 * @file MUWorkItemManager.cpp
 * @brief Thread pool work item container.
 *
 * @version 1.0
 * @date Mon Jul  2 16:10:22 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "MUWorkItemManager.h"

#include <map>

#include <inttypes.h>

#include "MUWorkItem.h"

MUWorkItemManager::~MUWorkItemManager()
{
    for (std::map<uint64_t, MUWorkItem *>::iterator it = m_ItemMap.begin();
         it != m_ItemMap.end();) {
        delete it->second;
        m_ItemMap.erase(it++);
    }
}

MUWorkItemManager::MUWorkItemManager()
{

}


bool
MUWorkItemManager::put(uint64_t itemId, MUWorkItem *pItem)
{
    std::pair<std::map<uint64_t, MUWorkItem *>::iterator, bool> rc;

    rc = m_ItemMap.insert(std::pair<uint64_t, MUWorkItem *>(itemId, pItem));

    if (!rc.second) {
        return false;
    }

    std::multimap<uint64_t, MUWorkItem *>::iterator it;
    it = m_ItemMapByBucket.insert(
             std::pair<uint64_t, MUWorkItem *>(pItem->getBucketId(), pItem));

    return true;
}

MUWorkItem *
MUWorkItemManager::get(uint64_t itemId)
{
    std::map<uint64_t, MUWorkItem *>::iterator it = m_ItemMap.find(itemId);

    return (m_ItemMap.end() == it) ? NULL : it->second;
}

void
MUWorkItemManager::remove(uint64_t itemId)
{
    MUWorkItem *pItem = get(itemId);

    if (NULL == pItem) {
        return ;
    }

    m_ItemMap.erase(itemId);

    std::pair < std::multimap<uint64_t, MUWorkItem *>::iterator,
        std::multimap<uint64_t, MUWorkItem *>::iterator > rt;
    rt = m_ItemMapByBucket.equal_range(pItem->getBucketId());

    for (std::multimap<uint64_t, MUWorkItem *>::iterator it = rt.first;
         it != rt.second; ++it) {

        if (pItem == it->second) {
            m_ItemMapByBucket.erase(it);
            return ;
        }
    }
}

bool
MUWorkItemManager::noItemsInBucket(uint64_t bucketId)
{
    std::multimap<uint64_t, MUWorkItem *>::iterator it;
    it = m_ItemMapByBucket.find(bucketId);

    if (m_ItemMapByBucket.end() == it) {
        return true;

    } else {
        return false;
    }
}

bool
MUWorkItemManager::noItems()
{
    if (0 == m_ItemMap.size()) {
        return true;

    } else {
        return false;
    }
}

void
MUWorkItemManager::getAllItems(std::list<uint64_t> *pItemList)
{
    for (std::map<uint64_t, MUWorkItem *>::iterator it = m_ItemMap.begin();
         it != m_ItemMap.end();) {
        pItemList->push_back(it->first);
    }
}

