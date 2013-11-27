#include "TCPSocket.h"
#include "Error.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <string.h>
//#include <sys/epoll.h>
#include <stdio.h>
//#include <linux/unistd.h>
//#include <linux/types.h>
//#include <linux/sysctl.h>
#include "BaseHeader.h"
#include "DevLog/DevLog.h"
#include "Tool.h"
extern DevLog *g_pDevLog;

int TCPSocket::ipType = IPV4;

TCPSocket::TCPSocket()
    : sockFd(-1)
{
}

TCPSocket::TCPSocket(int fd)
    : sockFd(fd)
{
}

int
TCPSocket::generateSocket(void)
{
    if( ipType == IPV4 )
        sockFd = socket(PF_INET, SOCK_STREAM, 0);
    else
        sockFd = socket(PF_INET6, SOCK_STREAM, 0);
    if (sockFd < 0) {
        DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::TCPSocket::socket");
        std::cout << strerror(errno) << std::endl;
    }

    return sockFd;
}

int
TCPSocket::getSockFd(void)const
{
    return sockFd;
}

int
TCPSocket::closeSocket(void)
{
    if( sockFd == -1)
        return 0;
    if (close(sockFd) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::closeSocket::close");
        return -1;
    }
    sockFd = -1;
    return 0;
}

int
TCPSocket::readSocket(char* buf, size_t len)
{
    int readNum = read(sockFd, buf, len);
    return readNum;
}

int
TCPSocket::writeSocket(const char * buf, size_t len)
{
    int writeNum = write(sockFd, buf, len);
    return writeNum;
}

int
TCPSocket::disableLinger(void)
{
    struct linger ling = {0, 0};
    if (setsockopt(sockFd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::disableLinger::setsockopt");
        return -1;
    }
    return 0;
}

int
TCPSocket::enableReuseaddr(void)
{
    int val = 1;
    if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, (const void*)&val, sizeof(val)) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::enableReuseaddr::setsockopt");
        return -1;
    }
    return 0;
}

int
TCPSocket::disableNagle(void)
{
    int val = 1;
    if (setsockopt(sockFd, IPPROTO_TCP, TCP_NODELAY, (const void*)&val, sizeof(val)) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::disableNagle::setsockopt");
        return -1;
    }
    return 0;
}

int
TCPSocket::setNonblock(void)
{
    int val;
    if ( (val = fcntl(sockFd, F_GETFL, 0)) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::setNonBlock::fcntl-F_GETFL");
        return val;
    }
    val |= O_NONBLOCK;
    if (fcntl(sockFd, F_SETFL, val) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::setNonBlock:fcntl");
        return -1;
    }
    return 0;
}

int
TCPSocket::bindAddr(SocketAddress& servaddr)
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
                DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::bindAddr::inet_addr: IP Address Invalid");
                return -1;
            }
        }

        if (servaddr.port == 0)
        {
            DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::bindAddr: Port Invalid");
            return -1;
        }
        addr.sin_port = htons(servaddr.port);
        if (bind(sockFd, (const struct sockaddr*)&addr, sizeof(addr)) < 0)
        {
            fprintf(stderr, "bind() error, %d, %s\n", sockFd, strerror(errno));
            //TASK_FALSE_MSG("TCPSocket::bindAddr : bind error, Port is ", servaddr.port);
            DEV_LOG(LERROR, OUT_BOTH, ("TCPSocket::bindAddr : bind error, Port is " + convertType<unsigned short, string>(servaddr.port)));

            return -1;
        }
    }
    else
    {
        struct sockaddr_in6 addr;
        memset( &addr, 0, sizeof( addr));
        addr.sin6_family = PF_INET6;
        addr.sin6_scope_id = 2;
        if( servaddr.ifAnyAddr())
            addr.sin6_addr = in6addr_any;
        else
        {
            if( inet_pton( PF_INET6, servaddr.getIP(), &( addr.sin6_addr)) < 0)
            {
                DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::bindAddr::inet_addr: IP Address Invalid");
                return -1;

            }
        }

        if( servaddr.port ==0 )
        {
            DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::bindAddr: Port Invalid");
            return -1;
        }
        addr.sin6_port = htons( servaddr.port);
        if(  bind( sockFd, (const struct sockaddr*)&addr, sizeof(addr)) < 0)
        {
            DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::bindAddr::bind");
            return -1;
        }

    }
    return 0;
}

int
TCPSocket::listenOn(int qs)
{
    if (listen(sockFd, qs) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::listenOn::listen");
        return -1;
    }
    return 0;
}

int
TCPSocket::acceptConnection(SocketAddress& addr)
{
    if( IPV4 == ipType )
    {
        struct sockaddr_in cliAddr;
        unsigned int cliAddrLen = sizeof(cliAddr);
        memset(&cliAddr, 0, cliAddrLen);
        int fd = accept(sockFd, (struct sockaddr*)&cliAddr, &cliAddrLen);
        addr.setAddress(inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
        return fd;
    }
    else
    {
        struct sockaddr_in6 cliAddr;
        unsigned int cliAddrLen = sizeof(cliAddr);
        memset(&cliAddr, 0, cliAddrLen);
        int fd = accept(sockFd, (struct sockaddr*)&cliAddr, &cliAddrLen);
        char str[46];
        addr.setAddress(inet_ntop(AF_INET6,&cliAddr.sin6_addr,str,sizeof(str)),
                        ntohs(cliAddr.sin6_port));
        return fd;
    }

}

int
TCPSocket::connectSocket(SocketAddress&  ipaddr)
{
    int ret = -1;
    if( ipType == IPV4 )
    {
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = PF_INET;
        if ( ipaddr.ifAnyAddr() )
        {
            DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::connect:ipaddr.ip NULL");
            return -1;
        }
        else
        {
            if ((addr.sin_addr.s_addr = inet_addr(ipaddr.getIP() )) == INADDR_NONE)
            {
                DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::bindAddr::inet_addr: IP Address Invalid");
                return -1;
            }
        }
        if (ipaddr.port == 0)
        {
            DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::connect:ipaddr.Port Invalid");
            return -1;
        }
        addr.sin_port = htons(ipaddr.port);

        ret = connect(sockFd, (struct sockaddr*)&addr, sizeof( addr));

    }
    else
    {
        struct sockaddr_in6 addr;
        memset( &addr, 0, sizeof(addr) );
        addr.sin6_family = PF_INET6;
        if( ipaddr.ifAnyAddr())
        {
            DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::connect:ipaddr.ip AnyAddr");
            return -1;
        }
        else
        {
            if( inet_pton( PF_INET6, ipaddr.getIP(), &(addr.sin6_addr)) < 0)
            {
                DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::bindAddr::inet_addr: IP Address Invalid");
                return -1;
            }
        }
        if( ipaddr.port == 0 )
        {
            DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::connect:ipaddr.Port Invalid");
            return -1;
        }
        addr.sin6_port = htons( ipaddr.port);
        addr.sin6_scope_id = 2;
        ret = connect( sockFd, (struct sockaddr*)&addr, sizeof( addr));
        if(ret<0)
            perror("TCPSocket::connecSocket");
    }
    return ret;

}

int
TCPSocket::writevSocket(const struct iovec *v, size_t c)
{
    int writeNum = writev(sockFd, v, c);
    return writeNum;
}

int
TCPSocket:: readnSocket(char *vptr, unsigned int n)
{
    unsigned int  nleft;
    int  nread;
    char *ptr;

    if (!vptr)
        return -1;
    ptr = vptr;
    nleft = n;

    while (nleft > 0)
    {
        if ((nread = read(sockFd, ptr, nleft)) < 0)
        {
            if (errno == EINTR)
                nread = 0;
            else if ( errno != EWOULDBLOCK )
            {
                perror("readn():");
                return -1;
            }
        }
        else if (nread == 0)
            break;
        else
        {
            nleft -= (unsigned int)nread;
            ptr += nread;
        }
    }

    return (int)(n - nleft);
}

int
TCPSocket::setKeepAlive( void)
{
    int val = 1;
    if (setsockopt(sockFd, SOL_SOCKET, SO_KEEPALIVE,
                   (const void*)&val, sizeof(val)) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "TCPSocket::setKeepAlive::setsockopt");
        return -1;
    }

    return 0;
}
//for debug
int TCPSocket::Peek(char* buf, size_t size)
{

    int readNum = recv(sockFd, buf, size, MSG_PEEK);

    return readNum;

//	printf("peeked: %d\n",count);
}
//for degug end
void TCPSocket::ThrowData(unsigned int size)
{
    char arr[size];
    if(read(sockFd, arr, size) < 0)
        return;
}
