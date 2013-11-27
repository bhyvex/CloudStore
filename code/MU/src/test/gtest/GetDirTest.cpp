/*
 * @file GetDirTest.cpp
 * @brief
 *
 * @version 1.0
 * @date Wed Aug  8 17:38:26 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "MUTest.h"

class GetDirTest : public MUTest
{
public:
    void getDir(uint64_t userId,
                const std::string &path,
                void (GetDirTest::*cb)()) {
        int rt = 0;

        _msg.cmd = MSG_SYS_MU_GET_DIR;

        cstore::pb_MSG_SYS_MU_GET_DIR getDir;
        getDir.set_uid(userId);
        getDir.set_token(USER_TOKEN);
        getDir.set_path(path);

        ASSERT_TRUE(getDir.SerializeToString(&_sendData));

        _msg.length = _sendData.length();

        rt = sendPacket(_msg, _sendData.c_str());
        ASSERT_EQ(0, rt);

        rt = readPacket(&_inReq);
        ASSERT_EQ(0, rt);

        ASSERT_EQ(MSG_SYS_MU_GET_DIR_ACK, _inReq.m_msgHeader.cmd);

        // callback
        (this->*cb)();
    }

    void cbNormally() {
        ASSERT_EQ(MU_SUCCESS, _inReq.m_msgHeader.error);
        //ASSERT_NE(0, _inReq.m_msgHeader.length);

        _readData = std::string(_inReq.ioBuf, _inReq.m_msgHeader.length);

        cstore::pb_MSG_SYS_MU_GET_DIR_ACK ack;

        ASSERT_TRUE(ack.ParseFromString(_readData));

        printf("total %d entries in dentry list\n", ack.dentry_list_size());

        for (int i = 0; i != ack.dentry_list_size(); ++i) {
            cstore::pb_MSG_SYS_MU_GET_DIR_ACK_DEntry entry =
                ack.dentry_list(i);
            if (entry.type() == MU_REGULAR_FILE) {
                printf("\t(file)\t");

            } else {
                printf("\t(dir )\t");
            }

            printf("%s\n", entry.name().c_str());
        }
    }
};

TEST_F(GetDirTest, Normally)
{
    getDir(VALID_USER_ID, "/not_empty_dir_00",
           &GetDirTest::cbNormally);
    getDir(VALID_USER_ID, "/empty_dir_00", &GetDirTest::cbNormally);
    getDir(VALID_USER_ID, "/",
           &GetDirTest::cbNormally);
}

TEST_F(GetDirTest, LocateError)
{
    getDir(INVALID_USER_ID, "/empty_dir_00", &GetDirTest::cbLocateError);
    getDir(NO_SUCH_USER_ID, "/empty_dir_01", &GetDirTest::cbLocateError);
}

TEST_F(GetDirTest, PathInvalid)
{
    getDir(VALID_USER_ID, "/no_such_dir/existed_dir_10",
           &GetDirTest::cbPathInvalid);
    getDir(VALID_USER_ID, "/empty_dir_00/",
           &GetDirTest::cbPathInvalid);
}

TEST_F(GetDirTest, PathNotExist)
{
    getDir(VALID_USER_ID, "/no_such_dir",
           &GetDirTest::cbPathNotExist);
    getDir(VALID_USER_ID, "/empty_dir_00/no_such_dir",
           &GetDirTest::cbPathNotExist);
}

TEST_F(GetDirTest, NotDirectory)
{
    getDir(VALID_USER_ID, "/existed_file_00",
           &GetDirTest::cbNotDirectory);
    getDir(VALID_USER_ID, "/not_empty_dir_00/existed_file_10",
           &GetDirTest::cbNotDirectory);
}

int
main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


