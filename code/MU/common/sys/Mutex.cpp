/*
 * =====================================================================================
 *
 *       Filename:  Mutex.cpp
 *
 *    Description:  pthread mutex封装
 *
 *        Version:  1.0
 *        Created:  04/25/2011 09:39:07 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#include "Mutex.h"

#include <cstdlib>
#include <cerrno>

#include <pthread.h>

#include "log/log.h"

Mutex::Mutex()
{
    pthread_mutexattr_t attr;

    int rt = pthread_mutexattr_init(&attr);

    if (0 != rt) {
        DEBUG_LOG("Syscall Error: pthread_mutexattr_init.");
    }

    rt = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);

    if (0 != rt) {
        DEBUG_LOG("Syscall Error: pthread_mutexattr_settype.");
    }

    rt = pthread_mutex_init(&m_Mutex, &attr);

    if (rt != 0) {
        DEBUG_LOG("Syscall Error: pthread_mutex_init.");
    }

    rt = pthread_mutexattr_destroy(&attr);

    if (0 != rt) {
        DEBUG_LOG("Syscall Error: pthread_mutexattr_destroy.");
    }
}

Mutex::~Mutex()
{
    const int rt = pthread_mutex_destroy(&m_Mutex);

    if (rt != 0) {
        DEBUG_LOG("Error occurred whilst destroying a mutex.");
    }
}

void Mutex::lock() const
{
    const int rt = pthread_mutex_lock(&m_Mutex);

    if (rt != 0) {
        if (rt == EDEADLK) {
            DEBUG_LOG(
                "Try to acquire the mutex when the current thread already owns the mutex. ");

        } else {
            DEBUG_LOG("Syscall Error: pthread_mutex_lock.");
        }
    }
}

bool Mutex::tryLock() const
{
    const int rt = pthread_mutex_trylock(&m_Mutex);

    if (rt != 0 && rt != EBUSY) {
        if (rt == EDEADLK) {
            DEBUG_LOG(
                "Try to acquire the mutex when the current thread already owns the mutex. ");

        } else {
            DEBUG_LOG("Syscall Error: pthread_mutex_trylock.");
        }

        return false;
    }

    return (rt == 0);
}

void Mutex::unlock() const
{
    const int rt = pthread_mutex_unlock(&m_Mutex);

    if (rt != 0) {
        DEBUG_LOG("Error occurred whilst unlock a mutex.");
    }
}

pthread_mutex_t *Mutex::getPthreadMutex() const
{
    return &m_Mutex;
}

//void Mutex::unlock(LockState *pLS) const
//{
//    pLS->mutex = &m_Mutex;
//}

//void Mutex::lock(LockState *pLS) const
//{
//}

LockGuard::~LockGuard()
{
    _pLock->unlock();
}

LockGuard::LockGuard(Mutex *pLock)
{
    _pLock = pLock;

    int rt = 0;

    _pLock->lock();
}



