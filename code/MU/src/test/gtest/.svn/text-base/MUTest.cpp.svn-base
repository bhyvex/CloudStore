/*
 * @file MUTest.cpp
 * @brief
 *
 * @version 1.0
 * @date Wed Aug  8 16:47:13 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "MUTest.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

MUTest::~MUTest()
{
    delete _pDataGen;
    _pDataGen = NULL;

    ::close(_connFd);
}

MUTest::MUTest()
{
    Xml *pXml = new Xml();

    xmlNode *pRoot = pXml->loadFile("TestConf.xml");

    if (NULL == pRoot) {
        ERROR_LOG("load configurations from TestConf.xml failed");
        exit(1);
    }

    int rt = 0;

    rt = pXml->getNodeValueByXPath("/TestConf/MU/ClientListenIP", &_ip);

    if (-1 == rt) {
        ERROR_LOG("parse mu ip from TestConf.xml failed");
        exit(1);
    }

    std::string port;
    rt = pXml->getNodeValueByXPath("/TestConf/MU/ClientListenPort", &port);

    if (-1 == rt) {
        ERROR_LOG("parse mu listen port from TestConf.xml failed");
        exit(1);
    }

    _port = util::conv::conv<uint16_t, std::string>(port);

    std::string dataRoot;
    rt = pXml->getNodeValueByXPath("/TestConf/MU/FileSystemRoot", &dataRoot);

    if (-1 == rt) {
        ERROR_LOG("parse filesystem root path from TestConf.xml failed");
        exit(1);
    }

    delete pXml;
    pXml = NULL;

    _dataRoot = dataRoot;

    _pDataGen = new DataGen(dataRoot);

    rt = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == rt) {
        printf("socket() error, %s\n", strerror(errno));
        exit(1);
    }

    _connFd = rt;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);

    rt = inet_pton(AF_INET, _ip.c_str(), &(addr.sin_addr));

    if (-1 == rt) {
        printf("inet_pton() error, %s\n", strerror(errno));
        exit(1);
    }

    rt = connect(_connFd, (struct sockaddr *) &addr, sizeof(addr));

    if (-1 == rt) {
        printf("connect() error, %s\n", strerror(errno));
        exit(1);
    }
}

int
MUTest::readPacket(InReq *pReq)
{
    int rt = 0;

    MsgHeader msg;

    rt = util::io::readn(_connFd, &msg, sizeof(msg));

    if (sizeof(msg) != rt) {
        printf("read() error, %s\n", strerror(errno));
        return -1;
    }

    pReq->m_msgHeader = msg;

    int len = msg.length;

    if (0 != len) {
        char *pBuf = new char[len];

        rt = util::io::readn(_connFd, pBuf, len);

        if (len != rt) {
            printf("read() error, %s\n", strerror(errno));
            return -1;
        }

        pReq->ioBuf = pBuf;
        pBuf = NULL;
    }

    return 0;
}

int
MUTest::sendPacket(const MsgHeader &msg, const char *pBuf)
{
    int rt = 0;

    rt = util::io::writen(_connFd, &msg, sizeof(msg));

    if (sizeof(msg) != rt) {
        printf("write() error when write header, %s\n", strerror(errno));
        return -1;
    }

    if (NULL != pBuf) {
        rt = util::io::writen(_connFd, pBuf, msg.length);

        if (msg.length != rt) {
            printf("write() error when write data, %s\n", strerror(errno));
            return -1;
        }
    }

    return 0;
}

void
MUTest::SetUp()
{
    int rt = 0;

    rt = _pDataGen->clean();

    if (-1 == rt) {
        WARN_LOG("clean data failed");
    }

    rt = _pDataGen->gen();

    if (-1 == rt) {
        ERROR_LOG("gen data failed");
        FAIL();
    }
}

void
MUTest::TearDown()
{
    int rt = 0;

    rt = _pDataGen->clean();

    if (-1 == rt) {
        WARN_LOG("clean data failed");
    }
}

