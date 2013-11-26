/*
 * =========================================================================
 *
 *       Filename:  UnxDmnSocket.h
 *
 *    Description:  Unix域套接字封装
 *
 *        Version:  1.0
 *        Created:  2012-01-12 19:36:34
 *  Last Modified:  2012-01-12 19:36:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#ifndef UNXDMNSOCKET_H_
#define UNXDMNSOCKET_H_

#include <string>

#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "UnxDmnSocketAddress.h"

/**
 * @brief Unix域套接字
 */
class UnxDmnSocket
{
public:
    UnxDmnSocket() {
        m_SockFd = -1;
    }

    explicit UnxDmnSocket(int sockFd) {
        m_SockFd = sockFd;
    }

    virtual ~UnxDmnSocket() {

    }

    void setFd(int Fd) {
        m_SockFd = Fd;
    }

    int getFd() {
        return m_SockFd;
    }

    /**
     * @brief 生成套接字
     *
     * @return  0 - success
     *         -1 - failed
     */
    int socket();

    int bind(const UnxDmnSocketAddress &addr);

    int listen(int backlog);

    int connect(const UnxDmnSocketAddress &addr);

    int accept(UnxDmnSocketAddress *pOppoAddr);

    int close();

    int setNonBlock();

    int read(char *pBuf, size_t len);

    /**
     * @brief 阻塞地读n个字节
     *
     * @param pBuf
     * @param len
     *
     * @return  实际读取的字节数
     */
    int readn(char *pBuf, size_t len);

    int write(const char *pBuf, size_t len);

    /**
     * @brief 阻塞写n个字节
     *
     * @param pBuf
     * @param len
     *
     * @return 实际写出的字节数
     */
    int writen(const char *pBuf, size_t len);

    /**
     * @brief 聚集写
     *
     * @param pIov 指向iovec链表第一个元素
     * @param iovCnt iovec链表长度
     *
     * @return 实际写出的字节数
     */
    int writev(const struct iovec *pIov, size_t iovCnt);

//public:
    //const int &m_cSockFd;

protected:
    // can't copy
    //UnxDmnSocket(const UnxDmnSocket &sock);
    //UnxDmnSocket &operator=(const UnxDmnSocket &sock);

private:
    int m_SockFd;
    UnxDmnSocketAddress m_LocalAddress;
    UnxDmnSocketAddress m_OppoAddress;
};

#endif  // UNXDMNSOCKET_H_

