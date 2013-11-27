/*
 * @file BucketManager.cpp
 * @brief MU hash rule manager.
 *
 * @version 1.0
 * @date Wed Jul  4 11:34:46 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "BucketManager.h"

#include <assert.h>

BucketManager::~BucketManager()
{
    for (std::map<uint64_t, Bucket *>::iterator it = m_BucketMap.begin();
         it != m_BucketMap.end();) {
        delete it->second;
        m_BucketMap.erase(it++);
    }

}

BucketManager::BucketManager()
{

}

void
BucketManager::getAllBuckets(std::list<Bucket *> *pBucketList)
{
    assert(pBucketList);

    for (std::map<uint64_t, Bucket *>::iterator it = m_BucketMap.begin();
         it != m_BucketMap.end(); ++it) {
        pBucketList->push_back(it->second);
    }
}


void
BucketManager::getAllBuckets(std::list<uint64_t> *pBucketList)
{
    assert(pBucketList);

    for (std::map<uint64_t, Bucket *>::iterator it = m_BucketMap.begin();
         it != m_BucketMap.end(); ++it) {
        pBucketList->push_back(it->second->m_BucketId);
    }
}

bool
BucketManager::put(uint64_t bucketId, Bucket *pBucket)
{
    std::pair<std::map<uint64_t, Bucket *>::iterator, bool> rc;

    rc = m_BucketMap.insert(std::pair<uint64_t, Bucket *>(bucketId, pBucket));

    return rc.second;
}

Bucket *
BucketManager::get(uint64_t bucketId)
{
    std::map<uint64_t, Bucket *>::iterator it = m_BucketMap.find(bucketId);

    return (m_BucketMap.end() == it) ? NULL : it->second;
}

void
BucketManager::remove(uint64_t bucketId)
{
    m_BucketMap.erase(bucketId);
}


