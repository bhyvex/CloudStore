/*
 * =========================================================================
 *
 *       Filename:  UnxDmnSocket.cpp
 *
 *    Description:  Unix套接字封装
 *
 *        Version:  1.0
 *        Created:  2012-01-12 22:57:50
 *  Last Modified:  2012-02-05 18:35:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#include "UnxDmnSocket.h"

#include <string>
#include <cstdlib>
#include <cstring>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/uio.h>

#include "log/log.h"

int
UnxDmnSocket::socket()
{
    int rt = 0;

    rt = ::socket(AF_LOCAL, SOCK_STREAM, 0);
    if (rt < 0) {
        ERROR_LOG("In UnxDmnSocket::socket, socket error: %s", 
                strerror(errno));
        return -1;
    }

    m_SockFd = rt;

    return 0;
}

int
UnxDmnSocket::bind(const UnxDmnSocketAddress &addr)
{
    int rt = 0;

    m_LocalAddress = addr;

    // failed if path not existed, ignore
    unlink(addr.m_cstrPath.c_str());  

    struct sockaddr_un localAddr = addr.m_cSockAddr;

    rt = ::bind(m_SockFd, 
            reinterpret_cast<struct sockaddr *>(&localAddr),
            SUN_LEN(&localAddr));
    if (rt < 0) {
        ERROR_LOG("In UnxDmnSocket::bind, bind error: %s",
                strerror(errno));
        return -1;
    }

    return 0;
}

int
UnxDmnSocket::listen(int backlog)
{
    int rt = 0;

    rt = ::listen(m_SockFd, backlog);
    if (rt < 0) {
        ERROR_LOG("In UnxDmnSocket::listen, listen error: %s",
                strerror(errno));
        return -1;
    }

    return 0;
}

int
UnxDmnSocket::connect(const UnxDmnSocketAddress &addr)
{
    int rt = 0;

    m_OppoAddress = addr;

    struct sockaddr_un oppoAddr = addr.m_cSockAddr;

    rt = ::connect(m_SockFd, 
            reinterpret_cast<struct sockaddr *>(&oppoAddr),
            SUN_LEN(&oppoAddr));
    if (rt < 0) {
        ERROR_LOG("In UnxDmnSocket::connect, connect error: %s",
                strerror(errno));
        return -1;
    }

    return 0;
}

int
UnxDmnSocket::accept(UnxDmnSocketAddress *pOppoAddr)
{
    int rt = 0;
    struct sockaddr_un oppoAddr;
    socklen_t len = 0;

    len = sizeof(oppoAddr);
    memset(&oppoAddr, 0, len);

    rt = ::accept(m_SockFd, 
            reinterpret_cast<struct sockaddr *>(&oppoAddr),
            &len);
    if (rt < 0) {
        ERROR_LOG("In UnxDmnSocket::accept, accept error: %s", 
                strerror(errno));
        return -1;
    }

    if (NULL != pOppoAddr) {
        pOppoAddr->setAddress(oppoAddr);
    }

    return rt;
}

int
UnxDmnSocket::close()
{
    int rt = 0;

    rt = ::close(m_SockFd);
    m_SockFd = -1;
    if (rt < 0) {
        WARN_LOG("In UnxDmnSocket::close, close error: %s",
                strerror(errno));
        return -1;
    }

    return 0;
}

int
UnxDmnSocket::setNonBlock()
{
    int val = 0;
    if ( (val = ::fcntl(m_SockFd, F_GETFL, 0)) < 0) {
        ERROR_LOG("In UnxDmnSocket::setNonBlock, fcntl(F_GETFL) error: %s",
                strerror(errno));
        return -1;
    }

    val |= O_NONBLOCK;

    if (::fcntl(m_SockFd, F_SETFL, val) < 0) {
        ERROR_LOG("In UnxDmnSocket::setNonBlock, fcntl(F_SETFL) error: %s",
                strerror(errno));
        return -1;
    }

    return 0;
}

int
UnxDmnSocket::read(char *pBuf, size_t len)
{
    if (NULL == pBuf) {
        FATAL_LOG("In UnxDmnSocket::read, pBuf == NULL");
        return -1;
    }

    int rt = 0;

    rt = ::read(m_SockFd, pBuf, len);
    if (rt < 0) {
        ERROR_LOG("In UnxDmnSocket::read, read error: %s", 
                strerror(errno));
        return -1;
    }

    return rt;
}

int
UnxDmnSocket::readn(char *pBuf, size_t len)
{
    if (NULL == pBuf) {
        FATAL_LOG("In UnxDmnSocket::readn, pBuf == NULL");
        return -1;
    }

    size_t nleft;
    ssize_t nread;
    char *ptr = NULL;

    nleft = len;
    nread = 0;
    ptr = pBuf;

    while (nleft > 0) {
        if ( (nread = ::read(m_SockFd, pBuf, nleft)) < 0) {
            // may error
            if (errno == EINTR) {
                // interrupt
                nread = 0;
                continue;
            } else {
                // error
                ERROR_LOG("In UnxDmnSocket::readn, read error: %s",
                        strerror(errno));
                return -1;
            }
        } else if (nread == 0) {
            // EOF
            break;
        }

        // some data read
        nleft -= nread;
        ptr += nread;
    }

    return (len - nleft);
}

int
UnxDmnSocket::write(const char *pBuf, size_t len)
{
    if (NULL == pBuf) {
        FATAL_LOG("In UnxDmnSocket::write, pBuf == NULL");
        return -1;
    }

    int rt = 0;
    rt = ::write(m_SockFd, pBuf, len);
    if (rt < 0) {
        ERROR_LOG("In UnxDmnSocket::write, write error: %s",
                strerror(errno));
        return -1;
    }

    return rt;
}

int
UnxDmnSocket::writen(const char *pBuf, size_t len)
{
    if (NULL == pBuf) {
        FATAL_LOG("In UnxDmnSocket::writen, pBuf == NULL");
        return -1;
    }

    size_t nleft;
    ssize_t nwritten;
    const char *ptr = NULL;

    nleft = len;
    nwritten = 0;
    ptr = pBuf;

    while (nleft > 0) {
        if ( (nwritten = ::write(m_SockFd, pBuf, nleft)) < 0) {
            // may error
            if (errno == EINTR) {
                // interrupt
                nwritten = 0;
                continue;
            } else {
                // error
                ERROR_LOG("In UnxDmnSocket::writen, write error: %s",
                        strerror(errno));
                return -1;
            }
        } else if (nwritten == 0) {
            break;
        }

        // some data written
        nleft -= nwritten;
        ptr += nwritten;
    }

    return (len - nleft);

}

int
UnxDmnSocket::writev(const struct iovec *pIov, size_t iovCnt)
{
    if (NULL == pIov) {
        FATAL_LOG("In UnxDmnSocket::writev, pIov == NULL");
        return -1;
    }

    int rt = 0;
    
    rt = ::writev(m_SockFd, pIov, iovCnt);
    if (rt < 0) {
        ERROR_LOG("In UnxDmnSocket::writev, writev error: %s",
                strerror(errno));
        return -1;
    }

    return rt;
}

