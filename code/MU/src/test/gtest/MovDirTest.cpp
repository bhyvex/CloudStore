/*
 * @file MovDirTest.cpp
 * @brief
 *
 * @version 1.0
 * @date Wed Aug  8 17:38:26 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "MUTest.h"

class MovDirTest : public MUTest
{
public:
    void movDir(uint64_t userId,
                const std::string &srcPath,
                const std::string &destPath,
                void (MovDirTest::*cb)()) {
        int rt = 0;

        _msg.cmd = MSG_SYS_MU_MOV_DIR;

        cstore::pb_MSG_SYS_MU_MOV_DIR movDir;
        movDir.set_uid(userId);
        movDir.set_token(USER_TOKEN);
        movDir.set_src_path(srcPath);
        movDir.set_dest_path(destPath);

        ASSERT_TRUE(movDir.SerializeToString(&_sendData));

        _msg.length = _sendData.length();

        rt = sendPacket(_msg, _sendData.c_str());
        ASSERT_EQ(0, rt);

        rt = readPacket(&_inReq);
        ASSERT_EQ(0, rt);

        ASSERT_EQ(MSG_SYS_MU_MOV_DIR_ACK, _inReq.m_msgHeader.cmd);

        // callback
        (this->*cb)();
    }

    void cbNormally() {
        ASSERT_EQ(MU_SUCCESS, _inReq.m_msgHeader.error);
        ASSERT_EQ(0, _inReq.m_msgHeader.length);
    }
};

TEST_F(MovDirTest, Normally)
{
    movDir(VALID_USER_ID,
           "/not_empty_dir_00/not_empty_dir_10",
           "/moved_not_empty_dir_00",
           &MovDirTest::cbNormally);
    movDir(VALID_USER_ID,
           "/empty_dir_00",
           "/moved_empty_dir_00",
           &MovDirTest::cbNormally);
    movDir(VALID_USER_ID,
           "/not_empty_dir_01",
           "/empty_dir_01",
           &MovDirTest::cbNormally);
}

TEST_F(MovDirTest, LocateError)
{
    movDir(INVALID_USER_ID,
           "/empty_dir_00",
           "/moved_empty_dir_00",
           &MovDirTest::cbLocateError);
    movDir(NO_SUCH_USER_ID,
           "/empty_dir_01",
           "/moved_empty_dir_00",
           &MovDirTest::cbLocateError);
}

TEST_F(MovDirTest, SrcPathInvalid)
{
    movDir(VALID_USER_ID,
           "/empty_dir_00/",
           "/moved_dir_00",
           &MovDirTest::cbSrcPathInvalid);
    movDir(VALID_USER_ID,
           "/no_such_dir/existed_dir_10",
           "/moved_dir_00",
           &MovDirTest::cbSrcPathInvalid);
    movDir(VALID_USER_ID,
           "/",
           "/moved_dir_00",
           &MovDirTest::cbSrcPathInvalid);
}

TEST_F(MovDirTest, SrcPathNotExist)
{
    movDir(VALID_USER_ID,
           "/no_such_dir",
           "/moved_dir_00",
           &MovDirTest::cbSrcPathNotExist);
    movDir(VALID_USER_ID,
           "/empty_dir_00/no_such_dir",
           "/moved_dir_00",
           &MovDirTest::cbSrcPathNotExist);
}

TEST_F(MovDirTest, SrcPathNotDirectory)
{
    movDir(VALID_USER_ID,
           "/existed_file_00",
           "/moved_dir_00",
           &MovDirTest::cbSrcPathNotDirectory);
    movDir(VALID_USER_ID,
           "/not_empty_dir_00/existed_file_10",
           "/moved_dir_00",
           &MovDirTest::cbSrcPathNotDirectory);
}

TEST_F(MovDirTest, DestPathInvalid)
{
    movDir(VALID_USER_ID,
           "/empty_dir_00",
           "/moved_dir_00/",
           &MovDirTest::cbDestPathInvalid);
    movDir(VALID_USER_ID,
           "/empty_dir_01",
           "/no_such_dir/moved_dir_10",
           &MovDirTest::cbDestPathInvalid);
    movDir(VALID_USER_ID,
           "/empty_dir_02",
           "/",
           &MovDirTest::cbDestPathInvalid);
}

TEST_F(MovDirTest, DirectoryNotEmpty)
{
    movDir(VALID_USER_ID,
           "/empty_dir_00",
           "/not_empty_dir_00",
           &MovDirTest::cbDirectoryNotEmpty);
    movDir(VALID_USER_ID,
           "/empty_dir_01",
           "/not_empty_dir_00/not_empty_dir_10",
           &MovDirTest::cbDirectoryNotEmpty);
}

TEST_F(MovDirTest, NotDirectory)
{
    movDir(VALID_USER_ID,
           "/empty_dir_00",
           "/existed_file_00",
           &MovDirTest::cbNotDirectory);
    movDir(VALID_USER_ID,
           "/empty_dir_01",
           "/not_empty_dir_00/existed_file_10",
           &MovDirTest::cbNotDirectory);
}

int
main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


