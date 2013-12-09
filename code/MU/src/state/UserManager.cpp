/*
 * @file UserManager.cpp
 * @brief user info manager.
 *
 * @version 1.0
 * @date Wed Jul  4 11:34:46 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "UserManager.h"

#include <assert.h>
#include <pthread.h>

#include "log/log.h"

#include "frame/MUMacros.h"

UserManager::~UserManager()
{
    int rt = 0;

    rt = ::pthread_rwlock_wrlock(&m_MapRWLock);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_wrlock() error, %s", strerror(rt));
    }

    for (std::map<uint64_t, User *>::iterator it = m_UserMap.begin();
         it != m_UserMap.end();) {
        delete it->second;
        m_UserMap.erase(it++);
    }

    rt = ::pthread_rwlock_unlock(&m_MapRWLock);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_unlock() error, %s", strerror(rt));
    }

    rt = ::pthread_rwlock_destroy(&m_MapRWLock);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_destroy() error, %s", strerror(rt));
    }
}

UserManager::UserManager()
{
    int rt = 0;

    rt = ::pthread_rwlock_init(&m_MapRWLock, NULL);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_init() error, %s", strerror(rt));
    }
}

void
UserManager::getAllUsers(std::list<User *> *pUserList)
{
    assert(pUserList);

    int rt = 0;

    rt = ::pthread_rwlock_rdlock(&m_MapRWLock);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_rdlock() error, %s", strerror(rt));
        return ;
    }

    for (std::map<uint64_t, User *>::iterator it = m_UserMap.begin();
         it != m_UserMap.end(); ++it) {
        pUserList->push_back(it->second);
    }

    rt = ::pthread_rwlock_unlock(&m_MapRWLock);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_unlock() error, %s", strerror(rt));
    }
}


void
UserManager::getAllUsers(std::list<uint64_t> *pUserList)
{
    assert(pUserList);

    int rt = 0;

    rt = ::pthread_rwlock_rdlock(&m_MapRWLock);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_rdlock() error, %s", strerror(rt));
        return ;
    }

    for (std::map<uint64_t, User *>::iterator it = m_UserMap.begin();
         it != m_UserMap.end(); ++it) {
        pUserList->push_back(it->second->m_UserId);
    }

    rt = ::pthread_rwlock_unlock(&m_MapRWLock);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_unlock() error, %s", strerror(rt));
    }
}

bool
UserManager::put(uint64_t userId, User *pUser)
{
    int rt = 0;

    rt = ::pthread_rwlock_wrlock(&m_MapRWLock);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_wrlock() error, %s", strerror(rt));
        return false;
    }

    std::pair<std::map<uint64_t, User *>::iterator, bool> rc;

    rc = m_UserMap.insert(std::pair<uint64_t, User *>(userId, pUser));

    rt = ::pthread_rwlock_unlock(&m_MapRWLock);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_unlock() error, %s", strerror(rt));
        // ignore this error
    }

    return rc.second;
}

User *
UserManager::get(uint64_t userId)
{
    int rt = 0;

    rt = ::pthread_rwlock_rdlock(&m_MapRWLock);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_rdlock() error, %s", strerror(rt));
        return NULL;
    }

    std::map<uint64_t, User *>::iterator it = m_UserMap.find(userId);

    if (m_UserMap.end() != it) {
        it->second->m_Mutex.lock();
        it->second->m_TimeOut = 0;
    }

    rt = ::pthread_rwlock_unlock(&m_MapRWLock);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_unlock() error, %s", strerror(rt));
    }

    if (m_UserMap.end() != it) {
        return it->second;
    }

    // no such user object, create a new one

    rt = ::pthread_rwlock_wrlock(&m_MapRWLock);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_wrlock() error, %s", strerror(rt));
        return NULL;
    }

    // lookup it again

    it = m_UserMap.find(userId);

    if (m_UserMap.end() != it) {
        it->second->m_Mutex.lock();
        it->second->m_TimeOut = 0;

        rt = ::pthread_rwlock_unlock(&m_MapRWLock);

        if (0 != rt) {
            ERROR_LOG("pthread_rwlock_unlock() error, %s", strerror(rt));
        }

        return it->second;
    }

    // new user

    User *pUser = new User();
    pUser->m_UserId = userId;
    pUser->m_Mutex.lock();

    std::pair<std::map<uint64_t, User *>::iterator, bool> rc;

    rc = m_UserMap.insert(std::pair<uint64_t, User *>(userId, pUser));

    rt = ::pthread_rwlock_unlock(&m_MapRWLock);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_unlock() error, %s", strerror(rt));
    }

    if (!rc.second) {
        ERROR_LOG("insert new user %" PRIu64 " into map failed",
                  userId);

        pUser->m_Mutex.unlock();
        delete pUser;
        pUser = NULL;

        return NULL;
    }

    return pUser;
}

void
UserManager::release(User *pUser)
{
    pUser->m_Mutex.unlock();
}


void
UserManager::remove(uint64_t userId)
{
    int rt = 0;

    rt = ::pthread_rwlock_wrlock(&m_MapRWLock);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_wrlock() error, %s", strerror(rt));
        return ;
    }

    m_UserMap.erase(userId);

    rt = ::pthread_rwlock_unlock(&m_MapRWLock);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_unlock() error, %s", strerror(rt));
        // ignore this error
    }
}

void
UserManager::checkIdleUsers()
{
    int rt = 0;
    int recycled = 0;

    rt = ::pthread_rwlock_wrlock(&m_MapRWLock);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_wrlock() error, %s", strerror(rt));
        return ;
    }

    TRACE_LOG("do idle user checking, %" PRIi32 " users now",
              m_UserMap.size());

    for (std::map<uint64_t, User *>::iterator it = m_UserMap.begin();
         it != m_UserMap.end();) {
        if (++(it->second->m_TimeOut) >= USER_IDLE_TIME_OUT) {
            it->second->m_Mutex.lock();
            it->second->m_Mutex.unlock();
            delete it->second;

            m_UserMap.erase(it++);
            ++recycled;
            continue;
        }

        ++it;
    }

    TRACE_LOG("%" PRIi32 " users recycled", recycled);

    rt = ::pthread_rwlock_unlock(&m_MapRWLock);

    if (0 != rt) {
        ERROR_LOG("pthread_rwlock_unlock() error, %s", strerror(rt));
        // ignore this error
    }
}


