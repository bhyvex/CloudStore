/*
 * @file FakeCS.cpp
 * @brief Fake CS module.
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
#include <list>

#include "protocol/protocol.h"
#include "comm/BaseHeader.h"
#include "comm/BaseReq.h"
#include "xml/Xml.h"

struct RuleItem {
    std::string ip0;
    std::string ip1;
    std::string ip2;
};

struct ListenThreadArg {
    std::string ip;
    uint16_t port;
};

static std::list<RuleItem> *g_pMURule;
static std::list<RuleItem> *g_pSURule;

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

void sendMURule(int fd)
{
    int rt = 0;

    MsgHeader msg;

    msg.cmd = MSG_SYS_RS_UPDATE_ALL_MU_HASH_ACK;
    msg.error = RULER_OK;

    cstore::pb_MSG_SYS_RS_UPDATE_ALL_MU_HASH_ACK ack;
    cstore::Map_Item *pItem = NULL;
    cstore::Hash_Version *pVersion = ack.mutable_hash_version();

    pVersion->set_hash_version(1);

    struct in_addr addr;

    for (std::list<RuleItem>::iterator it = g_pMURule->begin();
         it != g_pMURule->end(); ++it) {
        pItem = pVersion->add_map_list();

        rt = inet_pton(AF_INET, it->ip0.c_str(), &addr);

        if (-1 == rt) {
            printf("inet_pton() error, %s\n", strerror(errno));
            return ;
        }

        pItem->add_module_ip(addr.s_addr);

        rt = inet_pton(AF_INET, it->ip1.c_str(), &addr);

        if (-1 == rt) {
            printf("inet_pton() error, %s\n", strerror(errno));
            return ;
        }

        pItem->add_module_ip(addr.s_addr);
    }

    std::string data;

    if (!ack.SerializeToString(&data)) {
        printf("protobuf serialize error\n");
        return ;
    }

    msg.length = data.length();

    rt = sendPacket(fd, msg, data.c_str());

    if (-1 == rt) {
        printf("sendPacket() error\n");
        return ;
    }

}

void sendSURule(int fd)
{
    int rt = 0;

    MsgHeader msg;

    msg.cmd = MSG_SYS_RS_UPDATE_ALL_SU_HASH_ACK;
    msg.error = RULER_OK;

    cstore::pb_MSG_SYS_RS_UPDATE_ALL_SU_HASH_ACK ack;
    cstore::Map_Item *pItem = NULL;
    cstore::Hash_Version *pVersion = ack.mutable_hash_version();

    pVersion->set_hash_version(1);

    struct in_addr addr;

    for (std::list<RuleItem>::iterator it = g_pSURule->begin();
         it != g_pSURule->end(); ++it) {
        pItem = pVersion->add_map_list();

        rt = inet_pton(AF_INET, it->ip0.c_str(), &addr);

        if (-1 == rt) {
            printf("inet_pton() error, %s\n", strerror(errno));
            return ;
        }

        pItem->add_module_ip(addr.s_addr);

        rt = inet_pton(AF_INET, it->ip1.c_str(), &addr);

        if (-1 == rt) {
            printf("inet_pton() error, %s\n", strerror(errno));
            return ;
        }

        pItem->add_module_ip(addr.s_addr);

        rt = inet_pton(AF_INET, it->ip2.c_str(), &addr);

        if (-1 == rt) {
            printf("inet_pton() error, %s\n", strerror(errno));
            return ;
        }

        pItem->add_module_ip(addr.s_addr);
    }

    std::string data;

    if (!ack.SerializeToString(&data)) {
        printf("protobuf serialize error\n");
        return ;
    }

    msg.length = data.length();

    rt = sendPacket(fd, msg, data.c_str());

    if (-1 == rt) {
        printf("sendPacket() error\n");
        return ;
    }

}

void *rsProcessThread(void *pArg)
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

    if (packet.m_msgHeader.cmd == MSG_SYS_RS_UPDATE_ALL_MU_HASH) {
        cstore::pb_MSG_SYS_RS_UPDATE_ALL_MU_HASH req;

        if (!req.ParseFromString(data)) {
            printf("protobuf parse error\n");
            close(connFd);
            return (void *) - 1;
        }

        printf("local version %d\n", req.local_version());

        sendMURule(connFd);

    } else if (packet.m_msgHeader.cmd == MSG_SYS_RS_UPDATE_ALL_SU_HASH) {
        cstore::pb_MSG_SYS_RS_UPDATE_ALL_SU_HASH req;

        if (!req.ParseFromString(data)) {
            printf("protobuf parse error\n");
            close(connFd);
            return (void *) - 1;
        }

        printf("local version %d\n", req.local_version());

        sendSURule(connFd);

    } else {
        printf("unexpected protocol command 0x%x", packet.m_msgHeader.cmd);
    }

    delete [] packet.ioBuf;
    packet.ioBuf = NULL;

    ::close(connFd);

    return (void *) 0;
}

void *rsListenThread(void *pArg)
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
        return (void *) - 1;
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
                 &threadId, NULL, rsProcessThread, (void *) clientFd);

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

    g_pMURule = new std::list<RuleItem>();
    g_pSURule = new std::list<RuleItem>();

    // load config
    Xml *pXml = new Xml();

    xmlNode *pRoot = pXml->loadFile("FakeRSConf.xml");

    if (NULL == pRoot) {
        printf("load configurations from FakeRSConf.xml failed");
        exit(1);
    }

    std::list<xmlNode *> bucketNodes;
    std::list<xmlNode *> ipNodes;

    rt = pXml->getNodesByXPath("/Rule/MU/Bucket", &bucketNodes);

    if (-1 == rt) {
        printf("parse mu rules failed");
        exit(1);
    }

    for (std::list<xmlNode *>::iterator it = bucketNodes.begin();
         it != bucketNodes.end(); ++it) {
        ipNodes.clear();
        rt = pXml->getChildNodesByNodeName(*it, "IP", &ipNodes);

        if (-1 == rt) {
            printf("parse ip failed whilst parsing mu rules");
            exit(1);
        }

        RuleItem item;
        std::list<xmlNode *>::iterator nit = ipNodes.begin();
        rt = pXml->getNodeValue(*(nit++), &(item.ip0));

        if (-1 == rt) {
            printf("parse ip failed whilst parsing mu rules");
            exit(1);
        }

        rt = pXml->getNodeValue(*(nit++), &(item.ip1));

        if (-1 == rt) {
            printf("parse ip failed whilst parsing mu rules");
            exit(1);
        }

        g_pMURule->push_back(item);
    }

    bucketNodes.clear();

    rt = pXml->getNodesByXPath("/Rule/SU/Bucket", &bucketNodes);

    if (-1 == rt) {
        printf("parse su rules failed");
        exit(1);
    }

    for (std::list<xmlNode *>::iterator it = bucketNodes.begin();
         it != bucketNodes.end(); ++it) {
        ipNodes.clear();
        rt = pXml->getChildNodesByNodeName(*it, "IP", &ipNodes);

        if (-1 == rt) {
            printf("parse ip failed whilst parsing su rules");
            exit(1);
        }

        RuleItem item;
        std::list<xmlNode *>::iterator nit = ipNodes.begin();
        rt = pXml->getNodeValue(*(nit++), &(item.ip0));

        if (-1 == rt) {
            printf("parse ip failed whilst parsing su rules");
            exit(1);
        }

        rt = pXml->getNodeValue(*(nit++), &(item.ip1));

        if (-1 == rt) {
            printf("parse ip failed whilst parsing su rules");
            exit(1);
        }

        rt = pXml->getNodeValue(*(nit++), &(item.ip2));

        if (-1 == rt) {
            printf("parse ip failed whilst parsing su rules");
            exit(1);
        }

        g_pSURule->push_back(item);
    }

    delete pXml;
    pXml = NULL;

    struct ListenThreadArg *pArg = new ListenThreadArg();

    pArg->ip = argv[1];

    pArg->port = atoi(argv[2]);

    pthread_t threadId;

    rt = pthread_create(&threadId, NULL, rsListenThread, pArg);

    if (0 != rt) {
        printf("pthread_create() error, %s\n", strerror(errno));
        exit(1);
    }

    pause();

    return 0;
}


