/*
 * @file UserInfo.h
 * @brief User-related structures.
 *
 * @version 1.0
 * @date Thu Jul  5 15:40:41 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef UserInfo_H_
#define UserInfo_H_

#include <inttypes.h>

struct UserInfo {
    uint64_t m_TotalQuota;
    uint64_t m_UsedQuota;

    UserInfo() {
        m_TotalQuota = 0;
        m_UsedQuota = 0;
    }
};

#endif  // UserInfo_H_

