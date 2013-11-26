/*
 * @file Migrate.cpp
 * @brief Do migration.
 *
 * @version 1.0
 * @date Mon Jul 23 09:56:57 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>

#include "protocol/protocol.h"
#include "comm/BaseHeader.h"
#include "comm/BaseReq.h"

void usage(const char *pExecName)
{
    printf("%s MU_IP MU_Port New_Mod_Nr\n", pExecName);
}

int readPacket(int connFd, InReq *pReq)
{
    int rt = 0;

    MsgHeader msg;

    rt = read(connFd, &msg, sizeof(msg));

    if (sizeof(msg) != rt) {
        printf("read() error, %s\n", strerror(errno));
        return -1;
    }

    pReq->m_msgHeader = msg;

    int len = msg.length;

    if (0 != len) {
        char *pBuf = new char[len];

        rt = read(connFd, pBuf, len);

        if (len != rt) {
            printf("read() error, %s\n", strerror(errno));
            return -1;
        }

        pReq->ioBuf = pBuf;
        pBuf = NULL;
    }

    return 0;
}

int sendPacket(int connFd, const MsgHeader &msg, const char *pBuf)
{
    int rt = 0;

    rt = write(connFd, &msg, sizeof(msg));

    if (sizeof(msg) != rt) {
        printf("write() error when write header, %s\n", strerror(errno));
        return -1;
    }

    if (NULL != pBuf) {
        rt = write(connFd, pBuf, msg.length);

        if (msg.length != rt) {
            printf("write() error when write data, %s\n", strerror(errno));
            return -1;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int rt = 0;

    if (4 != argc) {
        usage(argv[0]);
        exit(1);
    }

    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    uint64_t modNr = atoi(argv[3]);


    // connect to mu

    rt = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == rt) {
        printf("socket() error, %s\n", strerror(errno));
        exit(1);
    }

    int sockFd = rt;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    rt = inet_pton(AF_INET, ip.c_str(), &(addr.sin_addr));

    if (-1 == rt) {
        printf("inet_pton() error, %s\n", strerror(errno));
        exit(1);
    }

    rt = connect(sockFd, (struct sockaddr *) &addr, sizeof(addr));

    if (-1 == rt) {
        printf("connect() error, %s\n", strerror(errno));
        exit(1);
    }

    MsgHeader msg;
    std::string data;
    InReq req;

    // prepare

    //msg.cmd = MSG_CS_MU_EXTENT_BUCKET_PREPARE;
    //msg.length = 0;

    //rt = sendPacket(sockFd, msg, NULL);
    //if (-1 == rt) {
        //printf("send packet extent_bucket_prepare failed\nn");
        //exit(1);
    //}

    //rt = readPacket(sockFd, &req);
    //if (-1 == rt) {
        //printf("read extent_bucket_prepare_ack failed\n");
        //exit(1);
    //}

    //if (MU_OK != req.m_msgHeader.error) {
        //printf("extent_bucket_prepare failed\n");
        //exit(1);
    //}

    // extent

    cstore::pb_MSG_CS_MU_EXTENT_BUCKET extentBucket;
    extentBucket.set_new_mod(modNr);

    if (!extentBucket.SerializeToString(&data)) {
        printf("protobuf serialize failed\n");
        exit(1);
    }

    msg.cmd = MSG_CS_MU_EXTENT_BUCKET;
    msg.length = data.length();

    rt = sendPacket(sockFd, msg, data.c_str());
    if (-1 == rt) {
        printf("send extent_bucket failed\n");
        exit(1);
    }

    rt = readPacket(sockFd, &req);
    if (-1 == rt) {
        printf("read extent_bucket_ack failed\n");
        exit(1);
    }

    if (MU_OK != req.m_msgHeader.error) {
        printf("extent_bucket failed\n");
        exit(1);
    }

    close(sockFd);

    printf("success!!!\n");

    return 0;
}


