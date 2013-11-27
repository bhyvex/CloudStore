/*
 * @file GetUserInfoTest.cpp
 * @brief
 *
 * @version 1.0
 * @date Wed Aug  8 17:38:26 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "MUTest.h"

class GetUserInfoTest : public MUTest
{
public:
    void getUserInfo(uint64_t userId, void (GetUserInfoTest::*cb)()) {
        int rt = 0;

        _msg.cmd = MSG_SYS_MU_GET_USER_INFO;

        cstore::pb_MSG_SYS_MU_GET_USER_INFO info;
        info.set_uid(userId);
        info.set_token(USER_TOKEN);

        ASSERT_TRUE(info.SerializeToString(&_sendData));

        _msg.length = _sendData.length();

        rt = sendPacket(_msg, _sendData.c_str());
        ASSERT_EQ(0, rt);

        rt = readPacket(&_inReq);
        ASSERT_EQ(0, rt);

        ASSERT_EQ(MSG_SYS_MU_GET_USER_INFO_ACK, _inReq.m_msgHeader.cmd);

        // callback
        (this->*cb)();
    }

    void cbNormally() {
        ASSERT_EQ(MU_SUCCESS, _inReq.m_msgHeader.error);
        ASSERT_NE(0, _inReq.m_msgHeader.length);

        _readData = std::string(_inReq.ioBuf, _inReq.m_msgHeader.length);

        cstore::pb_MSG_SYS_MU_GET_USER_INFO_ACK ack;

        ASSERT_TRUE(ack.ParseFromString(_readData));

        EXPECT_EQ(USER_QUOTA_TOTAL, ack.quota_total());
        EXPECT_EQ(USER_QUOTA_USED, ack.quota_used());
        EXPECT_EQ(USER_LOG_SEQ_NR, ack.seq_nr());
    }
};

TEST_F(GetUserInfoTest, Normally)
{
    getUserInfo(VALID_USER_ID, &GetUserInfoTest::cbNormally);
}

TEST_F(GetUserInfoTest, LocateError)
{
    getUserInfo(INVALID_USER_ID, &GetUserInfoTest::cbLocateError);
    getUserInfo(NO_SUCH_USER_ID, &GetUserInfoTest::cbLocateError);
}

int
main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


