#include "UDPSocket.h"
#include "Error.h"
#include <unistd.h>
#include <fcntl.h>
//#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
//#include <netinet/udp.h>
#include <arpa/inet.h>
#include <string.h>
//#include <sys/epoll.h>
#include <stdio.h>
//#include <linux/unistd.h>
//#include <linux/types.h>
//#include <linux/sysctl.h>
//#include "Header.h"
#include "DevLog/DevLog.h"
extern DevLog *g_pDevLog;


int UDPSocket::ipType = IPV4;

UDPSocket::UDPSocket()
    : sockFd(-1)
{
}


int UDPSocket::generateSocket(void)
{
    if( IPV4 == ipType )
        sockFd = socket(PF_INET, SOCK_DGRAM, 0);
    else
        sockFd = socket(PF_INET6, SOCK_DGRAM, 0);
    if (sockFd < 0)
        DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::TCPSocket::socket");
    return sockFd;
}

int UDPSocket::getSockFd(void)const
{
    return sockFd;
}

int UDPSocket::closeSocket(void)
{
    if( sockFd == -1)
        return 0;
    if (close(sockFd) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "UDPSocket::closeSocket::close");
        return -1;
    }
    sockFd = -1;
    return 0;
}

int UDPSocket::bindAddr(const SocketAddress& servaddr)
{
    if( ipType == IPV4 )
    {
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = PF_INET;

        if( servaddr.ifAnyAddr() )
            addr.sin_addr.s_addr = htonl( INADDR_ANY);
        else
        {
            if ((inet_aton(servaddr.getIP(), (in_addr*)&addr.sin_addr.s_addr)) == 0)
            {
                DEV_LOG(LERROR, OUT_BOTH, "UDPSocket::bindAddr::inet_addr: IP Address Invalid");
                return -1;
            }
        }

        if (servaddr.port == 0)
        {
            DEV_LOG(LERROR, OUT_BOTH, "UDPSocket::bindAddr: Port Invalid");
            return -1;
        }
        addr.sin_port = htons(servaddr.port);
        if (bind(sockFd, (const struct sockaddr*)&addr, sizeof(addr)) < 0)
        {
            DEV_LOG(LERROR, OUT_BOTH, "UDPSocket::bindAddr::bind");
            return -1;
        }
    }
    else
    {
        struct sockaddr_in6 addr;
        memset( &addr, 0, sizeof( addr));
        addr.sin6_family = PF_INET6;

        if( servaddr.ifAnyAddr())
            addr.sin6_addr = in6addr_any;
        else
        {
            if( inet_pton( PF_INET6, servaddr.getIP(), &( addr.sin6_addr)) < 0)
            {
                DEV_LOG(LERROR, OUT_BOTH, "UDPSocket::bindAddr::inet_addr: IP Address Invalid");
                return -1;
            }
        }
        if( servaddr.port ==0 )
        {
            DEV_LOG(LERROR, OUT_BOTH, "UDPSocket::bindAddr: Port Invalid");
            return -1;
        }
        addr.sin6_port = htons( servaddr.port);
        addr.sin6_scope_id = 2;
        if(  bind( sockFd, (const struct sockaddr*)&addr, sizeof(addr)) < 0)
        {
            DEV_LOG(LERROR, OUT_BOTH, "UDPSocket::bindAddr::bind");
            return -1;
        }

    }

    return 0;

}

int
UDPSocket::enableBroadcast(void)
{
    int val = 1;
    if (setsockopt(sockFd, SOL_SOCKET, SO_BROADCAST, (const void*)&val, sizeof(val)) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "UDPSocket::enableBroadcast::setsockopt");
        return FAILED;
    }
    return SUCCESSFUL;
}

int
UDPSocket::enableReuseaddr(void)
{
    int val = 1;
    if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, (const void*)&val, sizeof(val)) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "UDPSocket::enableReuseaddr::setsockopt");
        return -1;
    }
    return 0;
}

int
UDPSocket::setNonblock(void)
{
    int val;
    if ( (val = fcntl(sockFd, F_GETFL, 0)) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "UDPSocket::setNonBlock::fcntl");
        return val;
    }
    val |= O_NONBLOCK;
    if (fcntl(sockFd, F_SETFL, val) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "UDPSocket::setNonBlock:fcntl");
        return -1;
    }
    return 0;
}


int
UDPSocket::readSocket(char* buf, size_t len,struct sockaddr_in & cliaddr)
{
    socklen_t socklen = sizeof(cliaddr);

    int readNum = recvfrom( sockFd, buf, len,0,
                            (struct sockaddr *)&cliaddr,
                            (socklen_t *)&socklen);

    return readNum;
}

int
UDPSocket::writeSocket(const char * buf, size_t len,const struct sockaddr_in& cliaddr)
{
    int writeNum = sendto( sockFd, buf, len,0,
                           (const struct sockaddr *)&cliaddr,
                           (socklen_t)(sizeof(cliaddr)) );

    return writeNum;
}

