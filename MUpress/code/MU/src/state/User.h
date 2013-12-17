/*
 * @file User.h
 * @brief user infomation.
 *
 * @version 1.0
 * @date Tue Jul 31 14:38:25 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef STATE_USER_H_
#define STATE_USER_H_

#include <inttypes.h>

#include "sys/Mutex.h"

struct User {
    virtual ~User();

    User();

    // lock to make operations serializable
    Mutex m_Mutex;

    uint64_t m_UserId;

    // if user not active for a long time, this User object will be released
    int m_TimeOut;
};


#endif  // STATE_USER_H_

