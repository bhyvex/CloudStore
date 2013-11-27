/*
 * @file RWLock.h
 * @brief read-write lock
 *
 * @version 1.0
 * @date Tue Aug 14 09:49:07 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef COMMON_SYS_RWLOCK_H_
#define COMMON_SYS_RWLOCK_H_

#include <pthread.h>

class RWLock
{
public:
    virtual ~RWLock();

    RWLock();

    int rlock();

    int wlock();

    void unlock();

private:
    pthread_rwlock_t _rwlock;
};

class RWLockGuard
{
public:
    enum _LockType {
        READ,
        WRITE,
    };

public:
    virtual ~RWLockGuard();

    /**
     * @brief
     *
     * @param pLock get read lock? or write lock
     * @param type read lock or write lock
     */
    RWLockGuard(RWLock *pLock, _LockType type);

private:
    RWLock *_pLock;
};

#endif  // COMMON_SYS_RWLOCK_H_

