/*
 * @file net.cpp
 * @brief
 *
 * @version 1.0
 * @date Wed Aug 15 00:18:13 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "net.h"

#include <netinet/tcp.h>

namespace mu_test_tool_net
{

int
readPacket(int connFd, InReq *pReq)
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

int
sendPacket(int connFd, const MsgHeader &msg, const char *pBuf)
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


int
connectTo(const std::string &ip, uint16_t port)
{
    int rt = 0;

    struct in_addr addr;

    rt = inet_pton(AF_INET, ip.c_str(), &addr);

    if (-1 == rt) {
        printf("inet_pton() error, %s\n", strerror(errno));
        return -1;
    }

    return connectTo(addr, port);
}

int
connectTo(const struct in_addr &ip, uint16_t port)
{
    int rt = 0;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr = ip;
    addr.sin_port = htons(port);

    rt = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == rt) {
        printf("socket() error, %s\n", strerror(errno));
        return -1;
    }

    int connFd = rt;

    int value = 1;
    rt = setsockopt(connFd, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));
    if (-1 == rt) {
        printf("setsockopt() error, %s\n", strerror(errno));
        return -1;
    }

    rt = connect(
             connFd,
             (struct sockaddr *) &addr,
             sizeof(struct sockaddr_in)
         );

    if (-1 == rt) {
        printf("connect() error, %s\n", strerror(errno));
        close(connFd);
        return -1;
    }

    return connFd;
}


int
locate(
    const std::string &rsIp,
    uint16_t rsPort,
    uint64_t userId,
    struct in_addr *pAddr
)
{
    int rt = connectTo(rsIp, rsPort);
    if (-1 == rt) {
        printf("connect to rs at %s:%" PRIu16 " failed\n", 
                rsIp.c_str(), rsPort);
        return -1;
    }

    int sockFd = rt;

    MsgHeader msg;
    std::string data;
    InReq req;

    cstore::pb_MSG_SYS_RS_UPDATE_ALL_MU_HASH hash;
    hash.set_local_version(0);

    if (!hash.SerializeToString(&data)) {
        printf("protobuf serialize failed\n");
        return -1;
    }

    msg.cmd = MSG_SYS_RS_UPDATE_ALL_MU_HASH;
    msg.length = data.length();

    rt = sendPacket(sockFd, msg, data.c_str());

    if (-1 == rt) {
        printf("send update_all_mu_hash failed\n");
        return -1;
    }

    rt = readPacket(sockFd, &req);

    if (-1 == rt) {
        printf("read update_all_mu_hash_ack failed\n");
        return -1;
    }

    if (RULER_OK != req.m_msgHeader.error) {
        printf("update_all_mu_hash failed\n");
        return -1;
    }

    data = std::string(req.ioBuf, req.m_msgHeader.length);

    cstore::pb_MSG_SYS_RS_UPDATE_ALL_MU_HASH_ACK ack;

    if (!ack.ParseFromString(data)) {
        printf("protobuf parse failed\n");
        return -1;
    }

    cstore::Hash_Version version = ack.hash_version();
    cstore::Map_Item item;

    int totalBuckets = version.map_list_size();

    item = version.map_list(userId % totalBuckets);

    pAddr->s_addr = item.module_ip(0);

    return 0;
}


}  // namespace mu_test_tool_net


