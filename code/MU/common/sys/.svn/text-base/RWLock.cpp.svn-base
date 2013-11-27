/*
 * @file RWLock.cpp
 * @brief
 *
 * @version 1.0
 * @date Tue Aug 14 09:52:45 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "RWLock.h"

#include "log/log.h"

RWLock::~RWLock()
{
    int rt = 0;

    rt = ::pthread_rwlock_destroy(&_rwlock);

    if (-1 == rt) {
        ERROR_LOG("Syscall Error: pthread_rwlock_destroy, %s", strerror(rt));
    }
}

RWLock::RWLock()
{
    int rt = 0;

    rt = ::pthread_rwlock_init(&_rwlock, NULL);

    if (-1 == rt) {
        ERROR_LOG("Syscall Error: pthread_rwlock_init, %s", strerror(rt));
    }
}

int
RWLock::rlock()
{
    int rt = 0;

    rt = ::pthread_rwlock_rdlock(&_rwlock);

    if (-1 == rt) {
        DEBUG_LOG("Syscall Error: pthread_rwlock_rdlock, %s", strerror(rt));
        return -1;
    }

    return 0;
}

int
RWLock::wlock()
{
    int rt = 0;

    rt = ::pthread_rwlock_wrlock(&_rwlock);

    if (-1 == rt) {
        DEBUG_LOG("Syscall Error: pthread_rwlock_wrlock, %s", strerror(rt));
        return -1;
    }

    return 0;
}

void
RWLock::unlock()
{
    int rt = 0;

    rt = ::pthread_rwlock_unlock(&_rwlock);

    if (-1 == rt) {
        ERROR_LOG("Syscall Error: pthread_rwlock_unlock, %s", strerror(rt));
    }
}

RWLockGuard::~RWLockGuard()
{
    _pLock->unlock();
}

RWLockGuard::RWLockGuard(RWLock *pLock, _LockType type)
{
    _pLock = pLock;

    int rt = 0;

    if (READ == type) {
        rt = _pLock->rlock();

        if (-1 == rt) {
            DEBUG_LOG("acquire read lock failed");
        }

    } else {
        rt = _pLock->wlock();

        if (-1 == rt) {
            DEBUG_LOG("acquire write lock failed");
        }
    }
}



