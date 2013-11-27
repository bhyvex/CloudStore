/*
 * @file MUTest.h
 * @brief Base class for all MU test.
 *
 * @version 1.0
 * @date Wed Aug  8 16:28:23 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef GTEST_MUTEST_H_
#define GTEST_MUTEST_H_

#include <stdio.h>
#include <stdlib.h>

#include <string>

#include "gtest/gtest.h"

#include "comm/BaseHeader.h"
#include "comm/BaseReq.h"
#include "log/log.h"
#include "xml/Xml.h"
#include "util/util.h"
#include "protocol/protocol.h"
#include "frame/MUMacros.h"

#include "DataGen.h"

#define USER_TOKEN "2e4f8ad0"

#define INVALID_USER_ID     10001

#define NO_SUCH_USER_ID     10020

class MUTest : public testing::Test
{
public:
    virtual ~MUTest();

    MUTest();

    virtual void SetUp();

    virtual void TearDown();

public:
    void cbLocateError() {
        EXPECT_EQ(MU_LOCATE_ERROR, _inReq.m_msgHeader.error);
        EXPECT_EQ(0, _inReq.m_msgHeader.length);
    }

    void cbPathInvalid() {
        EXPECT_EQ(PATH_INVALID, _inReq.m_msgHeader.error);
        EXPECT_EQ(0, _inReq.m_msgHeader.length);
    }

    void cbPathExist() {
        EXPECT_EQ(PATH_EXIST, _inReq.m_msgHeader.error);
        EXPECT_EQ(0, _inReq.m_msgHeader.length);
    }

    void cbPathNotExist() {
        EXPECT_EQ(PATH_NOT_EXIST, _inReq.m_msgHeader.error);
        EXPECT_EQ(0, _inReq.m_msgHeader.length);
    }

    void cbNotDirectory() {
        EXPECT_EQ(NOT_DIRECTORY, _inReq.m_msgHeader.error);
        EXPECT_EQ(0, _inReq.m_msgHeader.length);
    }

    void cbSrcPathInvalid() {
        EXPECT_EQ(SRC_PATH_INVALID, _inReq.m_msgHeader.error);
        EXPECT_EQ(0, _inReq.m_msgHeader.length);
    }

    void cbSrcPathNotExist() {
        EXPECT_EQ(SRC_PATH_NOT_EXIST, _inReq.m_msgHeader.error);
        EXPECT_EQ(0, _inReq.m_msgHeader.length);
    }

    void cbSrcPathNotDirectory() {
        EXPECT_EQ(SRC_PATH_NOT_DIRECTORY, _inReq.m_msgHeader.error);
        EXPECT_EQ(0, _inReq.m_msgHeader.length);
    }

    void cbDestPathInvalid() {
        EXPECT_EQ(DEST_PATH_INVALID, _inReq.m_msgHeader.error);
        EXPECT_EQ(0, _inReq.m_msgHeader.length);
    }

    void cbDirectoryNotEmpty() {
        EXPECT_EQ(DIRECTORY_NOT_EMPTY, _inReq.m_msgHeader.error);
        EXPECT_EQ(0, _inReq.m_msgHeader.length);
    }

    void cbIsDirectory() {
        EXPECT_EQ(IS_DIRECTORY, _inReq.m_msgHeader.error);
        EXPECT_EQ(0, _inReq.m_msgHeader.length);
    }

    void cbSrcPathIsDirectory() {
        EXPECT_EQ(SRC_PATH_IS_DIRECTORY, _inReq.m_msgHeader.error);
        EXPECT_EQ(0, _inReq.m_msgHeader.length);
    }

    void cbDestPathIsDirectory() {
        EXPECT_EQ(DEST_PATH_IS_DIRECTORY, _inReq.m_msgHeader.error);
        EXPECT_EQ(0, _inReq.m_msgHeader.length);
    }

protected:
    int readPacket(InReq *pReq);

    int sendPacket(const MsgHeader &msg, const char *pBuf);

protected:
    DataGen *_pDataGen;
    std::string _dataRoot;
    std::string _ip;
    uint16_t _port;
    int _connFd;
    MsgHeader _msg;
    InReq _inReq;
    std::string _sendData;
    std::string _readData;
};

#endif  // GTEST_MUTEST_H_

