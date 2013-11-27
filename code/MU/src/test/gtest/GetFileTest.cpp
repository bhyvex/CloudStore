/*
 * @file GetFileTest.cpp
 * @brief
 *
 * @version 1.0
 * @date Wed Aug  8 17:38:26 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "MUTest.h"

class GetFileTest : public MUTest
{
public:
    void getFile(uint64_t userId,
                 const std::string &path,
                 void (GetFileTest::*cb)()) {
        int rt = 0;

        _msg.cmd = MSG_SYS_MU_GET_FILE;

        cstore::pb_MSG_SYS_MU_GET_FILE getFile;
        getFile.set_uid(userId);
        getFile.set_token(USER_TOKEN);
        getFile.set_path(path);

        ASSERT_TRUE(getFile.SerializeToString(&_sendData));

        _msg.length = _sendData.length();

        rt = sendPacket(_msg, _sendData.c_str());
        ASSERT_EQ(0, rt);

        rt = readPacket(&_inReq);
        ASSERT_EQ(0, rt);

        ASSERT_EQ(MSG_SYS_MU_GET_FILE_ACK, _inReq.m_msgHeader.cmd);

        // callback
        (this->*cb)();
    }

    void cbNormally() {
        ASSERT_EQ(MU_SUCCESS, _inReq.m_msgHeader.error);
        ASSERT_NE(0, _inReq.m_msgHeader.length);

        _readData = std::string (_inReq.ioBuf, _inReq.m_msgHeader.length);

        cstore::pb_MSG_SYS_MU_GET_FILE_ACK ack;

        ASSERT_TRUE(ack.ParseFromString(_readData));

        const cstore::File_Attr &attr = ack.attr();

        EXPECT_EQ(REGULAR_FILE_VERSION, attr.version());
        EXPECT_EQ(REGULAR_FILE_MODE, attr.mode());
        EXPECT_EQ(REGULAR_FILE_SIZE, attr.size());
        EXPECT_EQ(REGULAR_FILE_CTIME, attr.ctime());
        EXPECT_EQ(REGULAR_FILE_MTIME, attr.mtime());
        EXPECT_EQ(MU_REGULAR_FILE, attr.type());

        EXPECT_EQ(REGULAR_FILE_BLOCKS, ack.block_list_size());

        EXPECT_EQ(REGULAR_FILE_BLOCK_1, ack.block_list(0).checksum());
        EXPECT_EQ(REGULAR_FILE_BLOCK_2, ack.block_list(1).checksum());
        EXPECT_EQ(REGULAR_FILE_BLOCK_3, ack.block_list(2).checksum());
    }
};

TEST_F(GetFileTest, Normally)
{
    getFile(VALID_USER_ID, "/existed_file_00", &GetFileTest::cbNormally);
    getFile(VALID_USER_ID, "/not_empty_dir_00/existed_file_10",
            &GetFileTest::cbNormally);
}

TEST_F(GetFileTest, LocateError)
{
    getFile(INVALID_USER_ID, "/existed_file_00", &GetFileTest::cbLocateError);
    getFile(NO_SUCH_USER_ID, "/existed_file_01", &GetFileTest::cbLocateError);
}

TEST_F(GetFileTest, PathInvalid)
{
    getFile(VALID_USER_ID, "/no_such_dir/existed_file_10",
            &GetFileTest::cbPathInvalid);
    getFile(VALID_USER_ID, "/empty_dir_00/",
            &GetFileTest::cbPathInvalid);
    getFile(VALID_USER_ID, "/",
            &GetFileTest::cbPathInvalid);
}

TEST_F(GetFileTest, PathNotExist)
{
    getFile(VALID_USER_ID, "/no_such_file",
            &GetFileTest::cbPathNotExist);
    getFile(VALID_USER_ID, "/empty_dir_00/no_such_file",
            &GetFileTest::cbPathNotExist);
}

TEST_F(GetFileTest, IsDirectory)
{
    getFile(VALID_USER_ID, "/empty_dir_00",
            &GetFileTest::cbIsDirectory);
    getFile(VALID_USER_ID, "/not_empty_dir_00/not_empty_dir_10",
            &GetFileTest::cbIsDirectory);
}

int
main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


