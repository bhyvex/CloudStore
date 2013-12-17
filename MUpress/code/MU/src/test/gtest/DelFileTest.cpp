/*
 * @file DelFileTest.cpp
 * @brief
 *
 * @version 1.0
 * @date Wed Aug  8 17:38:26 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "MUTest.h"

class DelFileTest : public MUTest
{
public:
    void delFile(uint64_t userId,
                 const std::string &path,
                 void (DelFileTest::*cb)()) {
        int rt = 0;

        _msg.cmd = MSG_SYS_MU_DEL_FILE;

        cstore::pb_MSG_SYS_MU_DEL_FILE delFile;
        delFile.set_uid(userId);
        delFile.set_token(USER_TOKEN);
        delFile.set_path(path);

        ASSERT_TRUE(delFile.SerializeToString(&_sendData));

        _msg.length = _sendData.length();

        rt = sendPacket(_msg, _sendData.c_str());
        ASSERT_EQ(0, rt);

        rt = readPacket(&_inReq);
        ASSERT_EQ(0, rt);

        ASSERT_EQ(MSG_SYS_MU_DEL_FILE_ACK, _inReq.m_msgHeader.cmd);

        // callback
        (this->*cb)();
    }

    void cbNormally() {
        ASSERT_EQ(MU_SUCCESS, _inReq.m_msgHeader.error);
        ASSERT_EQ(0, _inReq.m_msgHeader.length);
    }
};

TEST_F(DelFileTest, Normally)
{
    delFile(VALID_USER_ID, "/existed_file_00", &DelFileTest::cbNormally);
    delFile(VALID_USER_ID, "/not_empty_dir_00/existed_file_10",
            &DelFileTest::cbNormally);
}

TEST_F(DelFileTest, LocateError)
{
    delFile(INVALID_USER_ID, "/existed_file_00", &DelFileTest::cbLocateError);
    delFile(NO_SUCH_USER_ID, "/existed_file_01", &DelFileTest::cbLocateError);
}

TEST_F(DelFileTest, PathInvalid)
{
    delFile(VALID_USER_ID, "/no_such_dir/existed_file_10",
            &DelFileTest::cbPathInvalid);
    delFile(VALID_USER_ID, "/empty_dir_00/",
            &DelFileTest::cbPathInvalid);
    delFile(VALID_USER_ID, "/",
            &DelFileTest::cbPathInvalid);
}

TEST_F(DelFileTest, PathNotExist)
{
    delFile(VALID_USER_ID, "/no_such_file",
            &DelFileTest::cbPathNotExist);
    delFile(VALID_USER_ID, "/empty_dir_00/no_such_file",
            &DelFileTest::cbPathNotExist);
}

TEST_F(DelFileTest, IsDirectory)
{
    delFile(VALID_USER_ID, "/empty_dir_00",
            &DelFileTest::cbIsDirectory);
    delFile(VALID_USER_ID, "/not_empty_dir_00/not_empty_dir_10",
            &DelFileTest::cbIsDirectory);
}

int
main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


