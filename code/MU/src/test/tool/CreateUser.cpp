/*
 * @file Migrate.cpp
 * @brief Create users as MPC.
 *
 * @version 1.0
 * @date Mon Jul 23 09:56:57 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
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
    printf("%s RS_IP RS_Port MU_Port UserID\n", pExecName);
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

int createUser(uint64_t userId, uint32_t ip, uint16_t port)
{
    int rt = 0;

    // connect to mu

    rt = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == rt) {
        printf("socket() error, %s\n", strerror(errno));
        return -1;
    }

    int sockFd = rt;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = ip;

    rt = connect(sockFd, (struct sockaddr *) &addr, sizeof(addr));

    if (-1 == rt) {
        printf("connect() error, %s\n", strerror(errno));
        return -1;
    }

    // create user

    MsgHeader msg;
    std::string data;
    InReq req;

    cstore::pb_MSG_MPC_MU_CREATE_USER createUser;
    createUser.set_uid(userId);
    createUser.set_quota(9999999999999999ULL);

    if (!createUser.SerializeToString(&data)) {
        printf("protobuf serialize failed\n");
        return -1;
    }

    msg.cmd = MSG_MPC_MU_CREATE_USER;
    msg.length = data.length();

    rt = sendPacket(sockFd, msg, data.c_str());

    if (-1 == rt) {
        printf("send create_user failed\nn");
        return -1;
    }

    rt = readPacket(sockFd, &req);

    if (-1 == rt) {
        printf("read create_user_ack failed\n");
        return -1;
    }

    if (MU_OK != req.m_msgHeader.error) {
        return -1;
    }

    ::close(sockFd);

    return 0;
}

int main(int argc, char *argv[])
{
    int rt = 0;

    if (5 != argc) {
        usage(argv[0]);
        exit(1);
    }

    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    uint64_t muport = atoi(argv[3]);
    uint64_t userId = atoll(argv[4]);

    // connect to rs

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

    // get rule

    MsgHeader msg;
    std::string data;
    InReq req;

    cstore::pb_MSG_SYS_RS_UPDATE_ALL_MU_HASH hash;
    hash.set_local_version(0);

    if (!hash.SerializeToString(&data)) {
        printf("protobuf serialize failed\n");
        exit(1);
    }

    msg.cmd = MSG_SYS_RS_UPDATE_ALL_MU_HASH;
    msg.length = data.length();

    rt = sendPacket(sockFd, msg, data.c_str());

    if (-1 == rt) {
        printf("send update_all_mu_hash failed\n");
        exit(1);
    }

    rt = readPacket(sockFd, &req);

    if (-1 == rt) {
        printf("read update_all_mu_hash_ack failed\n");
        exit(1);
    }

    if (RULER_OK != req.m_msgHeader.error) {
        printf("update_all_mu_hash failed\n");
        exit(1);
    }

    data = std::string(req.ioBuf, req.m_msgHeader.length);

    cstore::pb_MSG_SYS_RS_UPDATE_ALL_MU_HASH_ACK ack;

    if (!ack.ParseFromString(data)) {
        printf("protobuf parse failed\n");
        exit(1);
    }

    cstore::Hash_Version version = ack.hash_version();
    cstore::Map_Item item;

    int totalBuckets = version.map_list_size();

    // create user

    item = version.map_list(userId % totalBuckets);

    rt = createUser(userId, item.module_ip(0), muport);

    if (-1 == rt) {
        printf("create user %llu failed\n", userId);

    } else {
        printf("create user %llu success\n", userId);
    }

    return 0;
}


