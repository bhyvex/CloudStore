/*
 * @file RuleManager.h
 * @brief MU hash rule manager.
 *
 * @version 1.0
 * @date Wed Jul  4 11:01:34 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef RuleManager_H_
#define RuleManager_H_

#include "sys/sys.h"

#include <inttypes.h>
#include <math.h>

class RuleManager : public Singleton<RuleManager>
{
    friend class Singleton<RuleManager>;

public:
    virtual ~RuleManager();

public:
    inline uint64_t getTotalBuckets();

    inline uint64_t getLastTotalBuckets();

    inline void initModNr(uint64_t modNr);

    inline void setModNr(uint64_t modNr);

    inline uint64_t getModNr();

    inline uint64_t getLastModNr();

    /**
     * @brief Get users's bucketId.
     *
     * @param userId
     *
     * @return bucketId
     */
    inline uint64_t getBucketIdByUserId(uint64_t userId);

private:
    // can't new
    RuleManager();

private:
    uint64_t m_ModNr;
    uint64_t m_LastModNr;  // old mod nr
};

uint64_t 
RuleManager::getTotalBuckets()
{
    return ::pow(2, m_ModNr);
}

uint64_t
RuleManager::getLastTotalBuckets()
{
    return ::pow(2, m_LastModNr);
}

uint64_t
RuleManager::getModNr()
{
    return m_ModNr;
}

void
RuleManager::setModNr(uint64_t modNr)
{
    m_LastModNr = m_ModNr;
    m_ModNr = modNr;
}

void
RuleManager::initModNr(uint64_t modNr)
{
    m_LastModNr = modNr;
    m_ModNr = modNr;
}

uint64_t 
RuleManager::getLastModNr()
{
    return m_LastModNr;
}

uint64_t 
RuleManager::getBucketIdByUserId(uint64_t userId)
{
    return userId % getTotalBuckets();
}


#endif  // RuleManager_H_

