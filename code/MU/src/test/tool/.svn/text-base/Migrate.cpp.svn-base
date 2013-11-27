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
    printf("%s SRC_IP SRC_Port DEST_IP DEST_Port BucketId\n", pExecName);
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

    if (6 != argc) {
        usage(argv[0]);
        exit(1);
    }

    std::string srcIP = argv[1];
    uint16_t srcPort = atoi(argv[2]);
    std::string destIP = argv[3];
    uint16_t destPort = atoi(argv[4]);
    uint64_t bucketId = atoi(argv[5]);


    // connect to source mu

    rt = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == rt) {
        printf("socket() error, %s\n", strerror(errno));
        exit(1);
    }

    int srcFd = rt;

    struct sockaddr_in srcAddr;
    srcAddr.sin_family = AF_INET;
    srcAddr.sin_port = htons(srcPort);

    rt = inet_pton(AF_INET, srcIP.c_str(), &(srcAddr.sin_addr));

    if (-1 == rt) {
        printf("inet_pton() error, %s\n", strerror(errno));
        exit(1);
    }

    rt = connect(srcFd, (struct sockaddr *) &srcAddr, sizeof(srcAddr));

    if (-1 == rt) {
        printf("connect() error, %s\n", strerror(errno));
        exit(1);
    }

    // connect to dest mu

    rt = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == rt) {
        printf("socket() error, %s\n", strerror(errno));
        exit(1);
    }

    int destFd = rt;

    struct sockaddr_in destAddr;
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(destPort);

    rt = inet_pton(AF_INET, destIP.c_str(), &(destAddr.sin_addr));

    if (-1 == rt) {
        printf("inet_pton() error, %s\n", strerror(errno));
        exit(1);
    }

    rt = connect(destFd, (struct sockaddr *) &destAddr, sizeof(destAddr));

    if (-1 == rt) {
        printf("connect() error, %s\n", strerror(errno));
        exit(1);
    }

    std::string data;
    MsgHeader msg;
    InReq req;

    // stop bucket

    cstore::pb_MSG_CS_MU_SET_BUCKET_STATE stopBucket;
    stopBucket.set_state(MU_BUCKET_STOP);
    stopBucket.set_bucket_index(bucketId);

    if (!stopBucket.SerializeToString(&data)) {
        printf("protobuf serialize failed\n");
        exit(1);
    }

    msg.cmd = MSG_CS_MU_SET_BUCKET_STATE;
    msg.length = data.length();
    
    rt = sendPacket(srcFd, msg, data.c_str());
    if (-1 == rt) {
        printf("send packet stop_bucket failed\n");
        exit(1);
    }

    rt = readPacket(srcFd, &req);
    if (-1 == rt) {
        printf("read stop_bucket_ack failed\n");
        exit(1);
    }

    if (MU_OK != req.m_msgHeader.error) {
        printf("stop bucket failed\n");
        exit(1);
    }

    // do migration

    cstore::pb_MSG_CS_MU_MIGRATE_BUCKET migrateBucket;
    migrateBucket.set_bucket_index(bucketId);
    migrateBucket.set_source_mu(srcAddr.sin_addr.s_addr);

    if (!migrateBucket.SerializeToString(&data)) {
        printf("protobuf serialize failed\n");
        exit(1);
    }

    msg.cmd = MSG_CS_MU_MIGRATE_BUCKET;
    msg.length = data.length();

    rt = sendPacket(destFd, msg, data.c_str());
    if (-1 == rt) {
        printf("send packet migrate_bucket failed\n");
        exit(1);
    }

    rt = readPacket(destFd, &req);
    if (-1 == rt) {
        printf("read migrate_bucket_ack failed\n");
        exit(1);
    }

    if (MU_OK != req.m_msgHeader.error) {
        printf("migrate bucket failed\n");
        exit(1);
    }

    // delete bucket

    cstore::pb_MSG_CS_MU_DELETE_BUCKET deleteBucket;
    deleteBucket.set_bucket_index(bucketId);

    if (!deleteBucket.SerializeToString(&data)) {
        printf("protobuf serialize failed\n");
        exit(1);
    }

    msg.cmd = MSG_CS_MU_DELETE_BUCKET;
    msg.length = data.length();

    rt = sendPacket(srcFd, msg, data.c_str());
    if (-1 == rt) {
        printf("send packet delete_bucket failed\n");
        exit(1);
    }

    rt = readPacket(srcFd, &req);
    if (-1 == rt) {
        printf("read delete_bucket_ack failed\n");
        exit(1);
    }

    if (MU_OK != req.m_msgHeader.error) {
        printf("delete bucket failed\n");
        exit(1);
    }

    // start bucket
    
    cstore::pb_MSG_CS_MU_SET_BUCKET_STATE startBucket;
    startBucket.set_state(MU_BUCKET_START);
    startBucket.set_bucket_index(bucketId);

    if (!startBucket.SerializeToString(&data)) {
        printf("protobuf serialize failed\n");
        exit(1);
    }

    msg.cmd = MSG_CS_MU_SET_BUCKET_STATE;
    msg.length = data.length();
    
    rt = sendPacket(destFd, msg, data.c_str());
    if (-1 == rt) {
        printf("send packet start_bucket failed\n");
        exit(1);
    }

    rt = readPacket(destFd, &req);
    if (-1 == rt) {
        printf("read start_bucket_ack failed\n");
        exit(1);
    }

    if (MU_OK != req.m_msgHeader.error) {
        printf("start bucket failed\n");
        exit(1);
    }

    close(srcFd);
    close(destFd);

    printf("success!!!\n");

    return 0;
}


