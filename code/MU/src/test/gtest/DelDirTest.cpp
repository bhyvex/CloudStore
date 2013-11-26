/*
 * @file DelDirTest.cpp
 * @brief
 *
 * @version 1.0
 * @date Wed Aug  8 17:38:26 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "MUTest.h"

class DelDirTest : public MUTest
{
public:
    void delDir(uint64_t userId,
                const std::string &path,
                void (DelDirTest::*cb)()) {
        int rt = 0;

        _msg.cmd = MSG_SYS_MU_DEL_DIR;

        cstore::pb_MSG_SYS_MU_DEL_DIR delDir;
        delDir.set_uid(userId);
        delDir.set_token(USER_TOKEN);
        delDir.set_path(path);

        ASSERT_TRUE(delDir.SerializeToString(&_sendData));

        _msg.length = _sendData.length();

        rt = sendPacket(_msg, _sendData.c_str());
        ASSERT_EQ(0, rt);

        rt = readPacket(&_inReq);
        ASSERT_EQ(0, rt);

        ASSERT_EQ(MSG_SYS_MU_DEL_DIR_ACK, _inReq.m_msgHeader.cmd);

        // callback
        (this->*cb)();
    }

    void cbNormally() {
        ASSERT_EQ(MU_SUCCESS, _inReq.m_msgHeader.error);
        ASSERT_EQ(0, _inReq.m_msgHeader.length);
    }
};

TEST_F(DelDirTest, Normally)
{
    delDir(VALID_USER_ID, "/empty_dir_00", &DelDirTest::cbNormally);
    delDir(VALID_USER_ID, "/not_empty_dir_00", 
            &DelDirTest::cbNormally);
    delDir(VALID_USER_ID, "/", 
            &DelDirTest::cbNormally);
}

TEST_F(DelDirTest, LocateError)
{
    delDir(INVALID_USER_ID, "/empty_dir_00", &DelDirTest::cbLocateError);
    delDir(NO_SUCH_USER_ID, "/empty_dir_01", &DelDirTest::cbLocateError);
}

TEST_F(DelDirTest, PathInvalid)
{
    delDir(VALID_USER_ID, "/no_such_dir/existed_dir_10", 
            &DelDirTest::cbPathInvalid);
    delDir(VALID_USER_ID, "/empty_dir_00/", 
            &DelDirTest::cbPathInvalid);
}

TEST_F(DelDirTest, PathNotExist)
{
    delDir(VALID_USER_ID, "/no_such_dir",
            &DelDirTest::cbPathNotExist);
    delDir(VALID_USER_ID, "/empty_dir_00/no_such_dir",
            &DelDirTest::cbPathNotExist);
}

TEST_F(DelDirTest, NotDirectory)
{
    delDir(VALID_USER_ID, "/existed_file_00",
            &DelDirTest::cbNotDirectory);
    delDir(VALID_USER_ID, "/not_empty_dir_00/existed_file_10",
            &DelDirTest::cbNotDirectory);
}

int
main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


