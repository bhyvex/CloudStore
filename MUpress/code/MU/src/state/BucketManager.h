/*
 * @file BucketManager.h
 * @brief MU bucket manager.
 *
 * @version 1.0
 * @date Wed Jul  4 11:01:34 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef BucketManager_H_
#define BucketManager_H_

#include "sys/sys.h"

#include "Bucket.h"

#include <inttypes.h>

#include <map>
#include <list>

class BucketManager : public Singleton<BucketManager>
{
    friend class Singleton<BucketManager>;

public:
    virtual ~BucketManager();

    bool put(uint64_t bucketId, Bucket *pBucket);

    Bucket *get(uint64_t bucketId);

    void remove(uint64_t bucketId);

    void getAllBuckets(std::list<Bucket *> *pBucketList);

    void getAllBuckets(std::list<uint64_t> *pBucketList);

private:
    // can't new
    BucketManager();

private:
    std::map<uint64_t, Bucket *> m_BucketMap;
};


#endif  // BucketManager_H_

