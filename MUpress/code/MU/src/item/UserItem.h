/*
 * @file UserItem.h
 * @brief Thread pool request for user-related operations.
 *
 * @version 1.0
 * @date Tue Jul  3 15:07:27 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef UserItem_H_
#define UserItem_H_

#include "frame/MUWorkItem.h"
#include "frame/ReturnStatus.h"
#include "data/UserInfo.h"

class UserItem : public MUWorkItem
{
public:
    enum _UserItemWorkType {
        CREATE_USER,
        DELETE_USER,
        GET_USER_INFO
    };

public:
    virtual ~UserItem();

    explicit UserItem(MUTask *pTask);

    int process();

    void delay();

    inline uint32_t getRequestId();

    inline void setRequestId(uint32_t requestId);

    inline uint64_t getUserId();

    inline void setUserId(uint64_t userId);

    inline uint64_t getBucketId();

    inline void setBucketId(uint64_t bucketId);

    inline uint64_t getUserQuota();

    inline void setUserQuota(uint64_t quota);

    inline const UserInfo &getUserInfo();

    inline uint64_t getUserLogSeq();

protected:
    void createUser();

    void deleteUser();

    void readUserInfo();

    /**
     * @brief User already exists?
     *
     * @return 
     */
    ReturnStatus userExists();

    /**
     * @brief Get user root directory path.
     *
     * @return 
     */
    std::string userRoot();

private:
    // client request id
    uint32_t m_RequestId;

    // general
    uint64_t m_UserId;
    uint64_t m_BucketId;

    uint64_t m_UserQuota;

    // current user log seq
    uint64_t m_UserLogSeq;

    UserInfo m_UserInfo;
};


uint32_t
UserItem::getRequestId()
{
    return m_RequestId;
}

void
UserItem::setRequestId(uint32_t requestId)
{
    m_RequestId = requestId;
}

uint64_t
UserItem::getUserId()
{
    return m_UserId;
}

void
UserItem::setUserId(uint64_t userId)
{
    m_UserId = userId;
}

uint64_t
UserItem::getBucketId()
{
    return m_BucketId;
}

void
UserItem::setBucketId(uint64_t bucketId)
{
    m_BucketId = bucketId;
}

uint64_t
UserItem::getUserQuota()
{
    return m_UserQuota;
}

void
UserItem::setUserQuota(uint64_t quota)
{
    m_UserQuota = quota;
}

const UserInfo &
UserItem::getUserInfo()
{
    return m_UserInfo;
}

uint64_t
UserItem::getUserLogSeq()
{
    return m_UserLogSeq;
}

#endif  // UserItem_H_

