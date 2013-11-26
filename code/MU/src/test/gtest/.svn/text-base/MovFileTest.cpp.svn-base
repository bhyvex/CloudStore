/*
 * @file MovFileTest.cpp
 * @brief
 *
 * @version 1.0
 * @date Wed Aug  8 17:38:26 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "MUTest.h"

class MovFileTest : public MUTest
{
public:
    void movFile(uint64_t userId,
                 const std::string &srcPath,
                 const std::string &destPath,
                 void (MovFileTest::*cb)()) {
        int rt = 0;

        _msg.cmd = MSG_SYS_MU_MOV_FILE;

        cstore::pb_MSG_SYS_MU_MOV_FILE movFile;
        movFile.set_uid(userId);
        movFile.set_token(USER_TOKEN);
        movFile.set_src_path(srcPath);
        movFile.set_dest_path(destPath);

        ASSERT_TRUE(movFile.SerializeToString(&_sendData));

        _msg.length = _sendData.length();

        rt = sendPacket(_msg, _sendData.c_str());
        ASSERT_EQ(0, rt);

        rt = readPacket(&_inReq);
        ASSERT_EQ(0, rt);

        ASSERT_EQ(MSG_SYS_MU_MOV_FILE_ACK, _inReq.m_msgHeader.cmd);

        // callback
        (this->*cb)();
    }

    void cbNormally() {
        ASSERT_EQ(MU_SUCCESS, _inReq.m_msgHeader.error);
        ASSERT_EQ(0, _inReq.m_msgHeader.length);
    }
};

TEST_F(MovFileTest, Normally)
{
    movFile(VALID_USER_ID,
            "/not_empty_dir_00/existed_file_10",
            "/moved_file_00",
            &MovFileTest::cbNormally);
    movFile(VALID_USER_ID,
            "/existed_file_00",
            "/moved_file_01",
            &MovFileTest::cbNormally);
    movFile(VALID_USER_ID,
            "/existed_file_01",
            "/existed_file_02",
            &MovFileTest::cbNormally);
}

TEST_F(MovFileTest, LocateError)
{
    movFile(INVALID_USER_ID,
            "/existed_file_00",
            "/moved_file_00",
            &MovFileTest::cbLocateError);
    movFile(NO_SUCH_USER_ID,
            "/existed_file_00",
            "/moved_file_00",
            &MovFileTest::cbLocateError);
}

TEST_F(MovFileTest, SrcPathInvalid)
{
    movFile(VALID_USER_ID,
            "/empty_dir_00/",
            "/moved_file_00",
            &MovFileTest::cbSrcPathInvalid);
    movFile(VALID_USER_ID,
            "/no_such_dir/existed_file_10",
            "/moved_file_00",
            &MovFileTest::cbSrcPathInvalid);
    movFile(VALID_USER_ID,
            "/",
            "/moved_file_00",
            &MovFileTest::cbSrcPathInvalid);
}

TEST_F(MovFileTest, SrcPathNotExist)
{
    movFile(VALID_USER_ID,
            "/no_such_file",
            "/moved_file_00",
            &MovFileTest::cbSrcPathNotExist);
    movFile(VALID_USER_ID,
            "/empty_dir_00/no_such_file",
            "/moved_file_00",
            &MovFileTest::cbSrcPathNotExist);
}

TEST_F(MovFileTest, SrcPathIsDirectory)
{
    movFile(VALID_USER_ID,
            "/empty_dir_00",
            "/moved_file_00",
            &MovFileTest::cbSrcPathIsDirectory);
    movFile(VALID_USER_ID,
            "/not_empty_dir_00/not_empty_dir_10",
            "/moved_dir_00",
            &MovFileTest::cbSrcPathIsDirectory);
}

TEST_F(MovFileTest, DestPathInvalid)
{
    movFile(VALID_USER_ID,
            "/existed_file_00",
            "/moved_file_00/",
            &MovFileTest::cbDestPathInvalid);
    movFile(VALID_USER_ID,
            "/existed_file_01",
            "/no_such_dir/moved_file_10",
            &MovFileTest::cbDestPathInvalid);
    movFile(VALID_USER_ID,
            "/existed_file_02",
            "/",
            &MovFileTest::cbDestPathInvalid);
}

TEST_F(MovFileTest, DestPathIsDirectory)
{
    movFile(VALID_USER_ID,
            "/existed_file_00",
            "/empty_dir_00",
            &MovFileTest::cbDestPathIsDirectory);
    movFile(VALID_USER_ID,
            "/existed_file_01",
            "/not_empty_dir_00/empty_dir_10",
            &MovFileTest::cbDestPathIsDirectory);
    movFile(VALID_USER_ID,
            "/not_empty_dir_00/existed_file_10",
            "/not_empty_dir_01",
            &MovFileTest::cbDestPathIsDirectory);
}

int
main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


