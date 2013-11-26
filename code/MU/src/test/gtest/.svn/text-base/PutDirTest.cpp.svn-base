/*
 * @file PutDirTest.cpp
 * @brief
 *
 * @version 1.0
 * @date Wed Aug  8 17:38:26 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "MUTest.h"

class PutDirTest : public MUTest
{
public:
    void putDir(uint64_t userId,
                const std::string &path,
                void (PutDirTest::*cb)()) {
        int rt = 0;

        _msg.cmd = MSG_SYS_MU_PUT_DIR;

        cstore::pb_MSG_SYS_MU_PUT_DIR putDir;
        putDir.set_uid(userId);
        putDir.set_token(USER_TOKEN);
        putDir.set_path(path);

        ASSERT_TRUE(putDir.SerializeToString(&_sendData));

        _msg.length = _sendData.length();

        rt = sendPacket(_msg, _sendData.c_str());
        ASSERT_EQ(0, rt);

        rt = readPacket(&_inReq);
        ASSERT_EQ(0, rt);

        ASSERT_EQ(MSG_SYS_MU_PUT_DIR_ACK, _inReq.m_msgHeader.cmd);

        // callback
        (this->*cb)();
    }

    void cbNormally() {
        ASSERT_EQ(MU_SUCCESS, _inReq.m_msgHeader.error);
        ASSERT_EQ(0, _inReq.m_msgHeader.length);
    }
};

TEST_F(PutDirTest, Normally)
{
    putDir(VALID_USER_ID, "/new_dir_00", &PutDirTest::cbNormally);
    putDir(VALID_USER_ID, "/empty_dir_00/new_dir_10", 
            &PutDirTest::cbNormally);
}

TEST_F(PutDirTest, LocateError)
{
    putDir(INVALID_USER_ID, "/new_dir_00", &PutDirTest::cbLocateError);
    putDir(NO_SUCH_USER_ID, "/new_dir_01", &PutDirTest::cbLocateError);
}

TEST_F(PutDirTest, PathInvalid)
{
    putDir(VALID_USER_ID, "/no_such_dir/new_dir_10", 
            &PutDirTest::cbPathInvalid);
    putDir(VALID_USER_ID, "/", 
            &PutDirTest::cbPathInvalid);
    putDir(VALID_USER_ID, "/empty_dir_00/new_dir_10/", 
            &PutDirTest::cbPathInvalid);
}

TEST_F(PutDirTest, PathExist)
{
    putDir(VALID_USER_ID, "/empty_dir_00",
            &PutDirTest::cbPathExist);
    putDir(VALID_USER_ID, "/existed_file_00",
            &PutDirTest::cbPathExist);
    putDir(VALID_USER_ID, "/not_empty_dir_00/not_empty_dir_10",
            &PutDirTest::cbPathExist);
    putDir(VALID_USER_ID, "/not_empty_dir_00/existed_file_10",
            &PutDirTest::cbPathExist);
}

int
main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


