/*
 * @file Bucket.h
 * @brief To manage bucket state.
 *
 * @version 1.0
 * @date Thu Jul  5 19:12:01 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef Bucket_H_
#define Bucket_H_

#include "frame/MUWorkItemManager.h"

#include <inttypes.h>

struct Bucket {
    // To save Items which can not be executed immediately
    MUWorkItemManager *m_pItemManager;

    uint64_t m_BucketId;
    int m_BucketState;

    // current log sequence
    uint64_t m_LogSeq;

    virtual ~Bucket();

    Bucket();
};


#endif  // Bucket_H_

