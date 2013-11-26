/*
 * @file StatDirTest.cpp
 * @brief
 *
 * @version 1.0
 * @date Wed Aug  8 17:38:26 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "MUTest.h"

class StatDirTest : public MUTest
{
public:
    void statDir(uint64_t userId,
                const std::string &path,
                void (StatDirTest::*cb)()) {
        int rt = 0;

        _msg.cmd = MSG_SYS_MU_STAT_DIR;

        cstore::pb_MSG_SYS_MU_STAT_DIR statDir;
        statDir.set_uid(userId);
        statDir.set_token(USER_TOKEN);
        statDir.set_path(path);

        ASSERT_TRUE(statDir.SerializeToString(&_sendData));

        _msg.length = _sendData.length();

        rt = sendPacket(_msg, _sendData.c_str());
        ASSERT_EQ(0, rt);

        rt = readPacket(&_inReq);
        ASSERT_EQ(0, rt);

        ASSERT_EQ(MSG_SYS_MU_STAT_DIR_ACK, _inReq.m_msgHeader.cmd);

        // callback
        (this->*cb)();
    }

    void cbNormally1() {
        ASSERT_EQ(MU_SUCCESS, _inReq.m_msgHeader.error);
        ASSERT_NE(0, _inReq.m_msgHeader.length);

        _readData = std::string(_inReq.ioBuf, _inReq.m_msgHeader.length);

        cstore::pb_MSG_SYS_MU_STAT_DIR_ACK ack;

        ASSERT_TRUE(ack.ParseFromString(_readData));

        printf("stat for dir\n");

        printf("\tsize: %llu\n", ack.size());
        printf("\tmode: %llu\n", ack.mode());
        printf("\tctime: %llu\n", ack.ctime());
        printf("\tmtime: %llu\n", ack.mtime());

        struct stat st;

        std::string path = _dataRoot + PATH_SEPARATOR_STRING 
            + BUCKET_NAME(VALID_BUCKET_ID) 
            + PATH_SEPARATOR_STRING
            + USER_NAME(VALID_USER_ID) 
            + "/not_empty_dir_00";

        ASSERT_EQ(0, ::stat(path.c_str(), &st));

        EXPECT_EQ(st.st_size, ack.size());
        EXPECT_EQ(st.st_mode, ack.mode());
        EXPECT_EQ(st.st_ctime, ack.ctime());
        EXPECT_EQ(st.st_mtime, ack.mtime());
    }

    void cbNormally2()
    {
        ASSERT_EQ(MU_SUCCESS, _inReq.m_msgHeader.error);
        ASSERT_NE(0, _inReq.m_msgHeader.length);

        _readData = std::string(_inReq.ioBuf, _inReq.m_msgHeader.length);

        cstore::pb_MSG_SYS_MU_STAT_DIR_ACK ack;

        ASSERT_TRUE(ack.ParseFromString(_readData));

        printf("stat for dir\n");

        printf("\tsize: %llu\n", ack.size());
        printf("\tmode: %llu\n", ack.mode());
        printf("\tctime: %llu\n", ack.ctime());
        printf("\tmtime: %llu\n", ack.mtime());

        struct stat st;

        std::string path = _dataRoot + PATH_SEPARATOR_STRING 
            + BUCKET_NAME(VALID_BUCKET_ID) 
            + PATH_SEPARATOR_STRING
            + USER_NAME(VALID_USER_ID);

        ASSERT_EQ(0, ::stat(path.c_str(), &st));

        EXPECT_EQ(st.st_size, ack.size());
        EXPECT_EQ(st.st_mode, ack.mode());
        EXPECT_EQ(st.st_ctime, ack.ctime());
        EXPECT_EQ(st.st_mtime, ack.mtime());
    }
};

TEST_F(StatDirTest, Normally)
{
    statDir(VALID_USER_ID, "/not_empty_dir_00",
           &StatDirTest::cbNormally1);
    statDir(VALID_USER_ID, "/",
           &StatDirTest::cbNormally2);
}

TEST_F(StatDirTest, LocateError)
{
    statDir(INVALID_USER_ID, "/empty_dir_00", &StatDirTest::cbLocateError);
    statDir(NO_SUCH_USER_ID, "/empty_dir_01", &StatDirTest::cbLocateError);
}

TEST_F(StatDirTest, PathInvalid)
{
    statDir(VALID_USER_ID, "/no_such_dir/existed_dir_10",
           &StatDirTest::cbPathInvalid);
    statDir(VALID_USER_ID, "/empty_dir_00/",
           &StatDirTest::cbPathInvalid);
}

TEST_F(StatDirTest, PathNotExist)
{
    statDir(VALID_USER_ID, "/no_such_dir",
           &StatDirTest::cbPathNotExist);
    statDir(VALID_USER_ID, "/empty_dir_00/no_such_dir",
           &StatDirTest::cbPathNotExist);
}

TEST_F(StatDirTest, NotDirectory)
{
    statDir(VALID_USER_ID, "/existed_file_00",
           &StatDirTest::cbNotDirectory);
    statDir(VALID_USER_ID, "/not_empty_dir_00/existed_file_10",
           &StatDirTest::cbNotDirectory);
}

int
main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


