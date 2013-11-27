/*
 * @file UserManager.h
 * @brief user infomation manager
 *
 * @version 1.0
 * @date Wed Jul  4 11:01:34 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef STATE_USERMANAGER_H_
#define STATE_USERMANAGER_H_

#include <inttypes.h>
#include <pthread.h>

#include <map>
#include <list>

#include "sys/sys.h"

#include "User.h"

/**
 * @brief manager all active user infomation, accessed in thread pool
 */
class UserManager : public Singleton<UserManager>
{
    friend class Singleton<UserManager>;

public:
    virtual ~UserManager();

    bool put(uint64_t userId, User *pUser);

    User *get(uint64_t userId);

    void release(User *pUser);

    void remove(uint64_t userId);

    void checkIdleUsers();

    void getAllUsers(std::list<User *> *pUserList);

    void getAllUsers(std::list<uint64_t> *pUserList);

private:
    // can't new
    UserManager();

private:
    std::map<uint64_t, User *> m_UserMap;
    // protect user map
    pthread_rwlock_t m_MapRWLock;
};


#endif  // STATE_USERMANAGER_H_

