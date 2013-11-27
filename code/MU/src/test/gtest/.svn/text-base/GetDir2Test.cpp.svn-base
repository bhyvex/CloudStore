/*
 * @file GetDir2Test.cpp
 * @brief
 *
 * @version 1.0
 * @date Wed Aug  8 17:38:26 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "MUTest.h"

class GetDir2Test : public MUTest
{
public:
    void getDir2(uint64_t userId,
                 const std::string &path,
                 void (GetDir2Test::*cb)()) {
        int rt = 0;

        _msg.cmd = MSG_SYS_MU_GET_DIR2;

        cstore::pb_MSG_SYS_MU_GET_DIR2 getDir2;
        getDir2.set_uid(userId);
        getDir2.set_token(USER_TOKEN);
        getDir2.set_path(path);

        ASSERT_TRUE(getDir2.SerializeToString(&_sendData));

        _msg.length = _sendData.length();

        rt = sendPacket(_msg, _sendData.c_str());
        ASSERT_EQ(0, rt);

        rt = readPacket(&_inReq);
        ASSERT_EQ(0, rt);

        ASSERT_EQ(MSG_SYS_MU_GET_DIR2_ACK, _inReq.m_msgHeader.cmd);

        // callback
        (this->*cb)();
    }

    void cbNormally1() {
        ASSERT_EQ(MU_SUCCESS, _inReq.m_msgHeader.error);
        ASSERT_EQ(0, _inReq.m_msgHeader.length);
    }

    void cbNormally2() {
        ASSERT_EQ(MU_SUCCESS, _inReq.m_msgHeader.error);
        ASSERT_NE(0, _inReq.m_msgHeader.length);

        _readData = std::string(_inReq.ioBuf, _inReq.m_msgHeader.length);

        cstore::pb_MSG_SYS_MU_GET_DIR2_ACK ack;

        ASSERT_TRUE(ack.ParseFromString(_readData));

        printf("total %d entries in dentry list\n", ack.dentry_list_size());

        for (int i = 0; i != ack.dentry_list_size(); ++i) {
            cstore::pb_MSG_SYS_MU_GET_DIR2_ACK_DEntry entry =
                ack.dentry_list(i);

            if (entry.type() == MU_REGULAR_FILE) {
                printf("\t(file)\t");

            } else {
                printf("\t(dir )\t");
            }

            printf("%s\n", entry.name().c_str());

            if (entry.type() == MU_REGULAR_FILE) {
                printf("\t\tsize: %llu\n", entry.size());
                printf("\t\tmode: %llu\n", entry.mode());
                printf("\t\tctime: %llu\n", entry.ctime());
                printf("\t\tmtime: %llu\n", entry.mtime());
                printf("\t\tversion: %llu\n", entry.version());

                int blocks = entry.block_list_size();
                printf("\t\ttotal %d blocks\n", blocks);

                for (int j = 0; j < blocks; ++j) {
                    printf("\t\t\t%s\n",
                           entry.block_list(j).checksum().c_str());
                }

                EXPECT_EQ(REGULAR_FILE_SIZE, entry.size());
                EXPECT_EQ(REGULAR_FILE_MODE, entry.mode());
                EXPECT_EQ(REGULAR_FILE_CTIME, entry.ctime());
                EXPECT_EQ(REGULAR_FILE_MTIME, entry.mtime());
                EXPECT_EQ(REGULAR_FILE_VERSION, entry.version());
                EXPECT_EQ(REGULAR_FILE_BLOCKS, blocks);
            }
        }
    }
};

TEST_F(GetDir2Test, Normally)
{
    getDir2(VALID_USER_ID, "/empty_dir_00", &GetDir2Test::cbNormally1);
    getDir2(VALID_USER_ID, "/",
            &GetDir2Test::cbNormally2);
}

TEST_F(GetDir2Test, LocateError)
{
    getDir2(INVALID_USER_ID, "/empty_dir_00", &GetDir2Test::cbLocateError);
    getDir2(NO_SUCH_USER_ID, "/empty_dir_01", &GetDir2Test::cbLocateError);
}

TEST_F(GetDir2Test, PathInvalid)
{
    getDir2(VALID_USER_ID, "/no_such_dir/existed_dir_10",
            &GetDir2Test::cbPathInvalid);
    getDir2(VALID_USER_ID, "/empty_dir_00/",
            &GetDir2Test::cbPathInvalid);
}

TEST_F(GetDir2Test, PathNotExist)
{
    getDir2(VALID_USER_ID, "/no_such_dir",
            &GetDir2Test::cbPathNotExist);
    getDir2(VALID_USER_ID, "/empty_dir_00/no_such_dir",
            &GetDir2Test::cbPathNotExist);
}

TEST_F(GetDir2Test, NotDirectory)
{
    getDir2(VALID_USER_ID, "/existed_file_00",
            &GetDir2Test::cbNotDirectory);
    getDir2(VALID_USER_ID, "/not_empty_dir_00/existed_file_10",
            &GetDir2Test::cbNotDirectory);
}

int
main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


