/*
 * @file PutFileTest.cpp
 * @brief
 *
 * @version 1.0
 * @date Wed Aug  8 17:38:26 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "MUTest.h"

#include "data/FileMeta.h"

class PutFileTest : public MUTest
{
public:
    void putFile(uint64_t userId,
                const std::string &path,
                void (PutFileTest::*cb)(),
                uint64_t version = REGULAR_FILE_VERSION) {
        int rt = 0;

        _msg.cmd = MSG_SYS_MU_PUT_FILE;

        cstore::pb_MSG_SYS_MU_PUT_FILE putFile;
        putFile.set_uid(userId);
        putFile.set_token(USER_TOKEN);
        putFile.set_path(path);

        cstore::File_Attr *pAttr = putFile.mutable_attr();
        pAttr->set_version(version);
        pAttr->set_mode(REGULAR_FILE_MODE);
        pAttr->set_ctime(REGULAR_FILE_CTIME);
        pAttr->set_mtime(REGULAR_FILE_MTIME);
        pAttr->set_type(MU_REGULAR_FILE);
        pAttr->set_size(REGULAR_FILE_SIZE);

        cstore::Block_Meta *pBlock = NULL;
        pBlock = putFile.add_block_list();
        pBlock->set_checksum(REGULAR_FILE_BLOCK_1);
        pBlock = putFile.add_block_list();
        pBlock->set_checksum(REGULAR_FILE_BLOCK_2);
        pBlock = putFile.add_block_list();
        pBlock->set_checksum(REGULAR_FILE_BLOCK_3);

        ASSERT_TRUE(putFile.SerializeToString(&_sendData));

        _msg.length = _sendData.length();

        rt = sendPacket(_msg, _sendData.c_str());
        ASSERT_EQ(0, rt);

        rt = readPacket(&_inReq);
        ASSERT_EQ(0, rt);

        ASSERT_EQ(MSG_SYS_MU_PUT_FILE_ACK, _inReq.m_msgHeader.cmd);

        // callback
        (this->*cb)();
    }

    void cbNormally() {
        ASSERT_EQ(MU_SUCCESS, _inReq.m_msgHeader.error);
        ASSERT_EQ(0, _inReq.m_msgHeader.length);
    }

    void cbVersionOutdated() {
        ASSERT_EQ(VERSION_OUTDATED, _inReq.m_msgHeader.error);
        ASSERT_NE(0, _inReq.m_msgHeader.length);

        cstore::pb_MSG_SYS_MU_PUT_FILE_ACK ack;

        _readData = std::string(_inReq.ioBuf, _inReq.m_msgHeader.length);

        ASSERT_TRUE(ack.ParseFromString(_readData));

        EXPECT_EQ(5, ack.version());
    }

    void cbPathExist()
    {
        ASSERT_EQ(PATH_EXIST, _inReq.m_msgHeader.error);
        ASSERT_NE(0, _inReq.m_msgHeader.length);

        cstore::pb_MSG_SYS_MU_PUT_FILE_ACK ack;

        _readData = std::string(_inReq.ioBuf, _inReq.m_msgHeader.length);

        ASSERT_TRUE(ack.ParseFromString(_readData));

        EXPECT_EQ(0, ack.version());
    }
};

TEST_F(PutFileTest, Normally)
{
    putFile(VALID_USER_ID, "/new_file_00", &PutFileTest::cbNormally);
    putFile(VALID_USER_ID, "/empty_dir_00/new_file_10", 
            &PutFileTest::cbNormally);
}

TEST_F(PutFileTest, LocateError)
{
    putFile(INVALID_USER_ID, "/new_file_00", &PutFileTest::cbLocateError);
    putFile(NO_SUCH_USER_ID, "/new_file_01", &PutFileTest::cbLocateError);
}

TEST_F(PutFileTest, PathInvalid)
{
    putFile(VALID_USER_ID, "/no_such_dir/new_file_10", 
            &PutFileTest::cbPathInvalid);
    putFile(VALID_USER_ID, "/empty_dir_00/new_file_10/", 
            &PutFileTest::cbPathInvalid);
    putFile(VALID_USER_ID, "/", 
            &PutFileTest::cbPathInvalid);
}

TEST_F(PutFileTest, PathNotExist)
{
    putFile(VALID_USER_ID, "/new_file_00", 
            &PutFileTest::cbPathNotExist, 1);
    putFile(VALID_USER_ID, "/empty_dir_00/new_file_10", 
            &PutFileTest::cbPathNotExist, 1);
}

TEST_F(PutFileTest, PathExist)
{
    putFile(VALID_USER_ID, "/existed_file_00",
            &PutFileTest::cbPathExist);
    putFile(VALID_USER_ID, "/not_empty_dir_00/existed_file_10",
            &PutFileTest::cbPathExist);
}

TEST_F(PutFileTest, IsDirectory)
{
    putFile(VALID_USER_ID, "/empty_dir_00", 
            &PutFileTest::cbIsDirectory);
    putFile(VALID_USER_ID, "/empty_dir_00", 
            &PutFileTest::cbIsDirectory, 3);
}

TEST_F(PutFileTest, VersionOutdated)
{
    std::string path = "/not_empty_dir_00/existed_file_10";
    std::string rpath = _dataRoot 
        + PATH_SEPARATOR_STRING
        + BUCKET_NAME(VALID_BUCKET_ID)
        + PATH_SEPARATOR_STRING
        + USER_NAME(VALID_USER_ID)
        + path;

    int rt = 0;

    FileAttr attr;

    rt = ::open(rpath.c_str(), O_RDWR);
    ASSERT_NE(-1, rt);

    int fd = rt;

    ASSERT_EQ(sizeof(FileAttr), ::read(fd, &attr, sizeof(FileAttr)));

    attr.m_Version = 5;

    ASSERT_NE(-1, ::lseek(fd, 0, SEEK_SET));

    ASSERT_EQ(sizeof(FileAttr), ::write(fd, &attr, sizeof(FileAttr)));

    ::close(fd);

    putFile(VALID_USER_ID, path,
            &PutFileTest::cbVersionOutdated, 3);
    putFile(VALID_USER_ID, path,
            &PutFileTest::cbVersionOutdated, 8);
}

int
main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


