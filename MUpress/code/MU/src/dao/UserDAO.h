/*
 * @file UserDAO.h
 * @brief User-related data access object.
 *
 * @version 1.0
 * @date Thu Jul  5 16:03:06 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef UserDAO_H_
#define UserDAO_H_

#include "frame/MUDAO.h"
#include "frame/ReturnStatus.h"
#include "data/UserInfo.h"
#include "storage/NameSpace.h"


#include <inttypes.h>

#include <string>

class UserDAO : public MUDAO
{
public:
    virtual ~UserDAO();

    UserDAO();

    ReturnStatus createUser(uint64_t bucketId,
                            uint64_t userId, uint64_t quota);

    ReturnStatus deleteUser(uint64_t bucketId, uint64_t userId);

    ReturnStatus readUserInfo(uint64_t bucketId,
                              uint64_t userId, UserInfo *pInfo);

    ReturnStatus writeUserInfo(uint64_t bucketId,
                               uint64_t userId, const UserInfo &info);

protected:
    ReturnStatus readUserInfo(Args *fd, UserInfo *pInfo);

    ReturnStatus writeUserInfo(Args *fd, const UserInfo &info);

    /**
     * @brief Delete a specified user, even it's no empty.
     *
     *
     * @return
     */
    ReturnStatus rmUserRecursive(uint64_t bucketId, uint64_t userId);

    std::string absUserRootPath(uint64_t bucketId, uint64_t userId);
};

#endif  // UserDAO_H_


