/*
 * @file FakeCS.cpp
 * @brief Fake CS module.
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

struct ListenThreadArg {
    std::string ip;
    uint16_t port;
};

void usage(const char *pExecName)
{
    printf("%s IP Port\n", pExecName);
}

int reuseAddr(int fd)
{
    int rt = 0;

    int val = 1;
    rt = setsockopt(
             fd,
             SOL_SOCKET,
             SO_REUSEADDR,
             (const void *) &val,
             sizeof(val));

    if (-1 == rt) {
        printf("setsockopt() error, %s", strerror(errno));
        return -1;
    }

    return 0;
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

void *heartbeatProcessThread(void *pArg)
{
    int connFd = (int) pArg;
    int rt = 0;

    InReq packet;

    rt = readPacket(connFd, &packet);

    if (-1 == rt) {
        printf("readPacket() error");
        close(connFd);
        return (void *) - 1;
    }

    std::string data(packet.ioBuf, packet.m_msgHeader.length);

    delete [] packet.ioBuf;
    packet.ioBuf = NULL;

    cstore::pb_MSG_MU_CS_HEARTBEAT_HANDSHAKE req;

    if (!req.ParseFromString(data)) {
        printf("protobuf parse error\n");
        close(connFd);
        return (void *) - 1;
    }

    printf("storage limit %ld\n", req.storage_load_limit());
    printf("\n");

    MsgHeader msg;

    msg.cmd = MSG_MU_CS_HEARTBEAT_HANDSHAKE_ACK;
    msg.length = 0;
    msg.error = CS_OK;

    rt = sendPacket(connFd, msg, NULL);

    if (-1 == rt) {
        printf("sendPacket() error\n");
        close(connFd);
        return (void *) - 1;
    }

    cstore::pb_MSG_MU_CS_HEARTBEAT heartbeat;
    cstore::Bucket_Item item;

    for (; ;) {
        rt = readPacket(connFd, &packet);

        if (-1 == rt) {
            printf("readPacket() error\n");
            close(connFd);
            return (void *) - 1;
        }

        std::string data(packet.ioBuf, packet.m_msgHeader.length);

        delete [] packet.ioBuf;
        packet.ioBuf = NULL;

        if (!heartbeat.ParseFromString(data)) {
            printf("protobuf parse error\n");
            close(connFd);
            return (void *) - 1;
        }

        for (int i = 0; i < heartbeat.bucket_item_size(); ++i) {
            item = heartbeat.bucket_item(i);
            printf("bucket %ld, current storage %ld\n",
                   item.bucket(), item.bucket_load());
        }

        MsgHeader msg;
        msg.cmd = MSG_MU_CS_HEARTBEAT_ACK;
        msg.length = 0;
        msg.error = CS_OK;

        rt = sendPacket(connFd, msg, NULL);

        if (-1 == rt) {
            printf("sendPacket() error\n");
            close(connFd);
            return (void *) - 1;
        }
        printf("\n");
    }
}

void *heartbeatListenThread(void *pArg)
{
    int rt = 0;

    struct ListenThreadArg *pRArg = (struct ListenThreadArg *) pArg;

    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(pRArg->port);

    rt = inet_pton(AF_INET, pRArg->ip.c_str(), &(addr.sin_addr.s_addr));

    if (-1 == rt) {
        printf("inet_pton() error, %s\n", strerror(errno));
        return (void *) - 1;
    }

    delete(struct ListenThreadArg *) pArg;

    rt = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == rt) {
        printf("socket() error, %s\n", strerror(errno));
        return (void *) - 1;
    }

    int listenFd = rt;

    rt = reuseAddr(listenFd);
    if (-1 == rt) {
        printf("set reuse_addr failed");
        return (void *) -1;
    }

    rt = ::bind(listenFd, (struct sockaddr *) &addr, sizeof(addr));

    if (-1 == rt) {
        printf("bind() error, %s\n", strerror(errno));
        return (void *) - 1;
    }

    rt = listen(listenFd, 100);

    if (-1 == rt) {
        printf("listen() error, %s\n", strerror(errno));
        return (void *) - 1;
    }

    int clientFd = 0;
    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    pthread_t threadId;

    for (; ;) {
        clientFd =
            accept(listenFd, (struct sockaddr *) &clientAddr, &addrLen);

        if (-1 == clientFd) {
            printf("accept() error, %s\n", strerror(errno));
            continue;
        }

        rt = pthread_create(
                 &threadId, NULL, heartbeatProcessThread, (void *) clientFd);

        if (0 != rt) {
            printf("pthread_create() error, %s\n", strerror(rt));
            return (void *) - 1;
        }
    }

    return (void *) 0;
}

int main(int argc, char *argv[])
{
    int rt = 0;

    if (3 != argc) {
        usage(argv[0]);
        exit(1);
    }

    struct ListenThreadArg *pArg = new ListenThreadArg();

    pArg->ip = argv[1];

    pArg->port = atoi(argv[2]);

    pthread_t threadId;

    rt = pthread_create(&threadId, NULL, heartbeatListenThread, pArg);

    if (0 != rt) {
        printf("pthread_create() error, %s\n", strerror(errno));
        exit(1);
    }

    pause();

    return 0;
}


