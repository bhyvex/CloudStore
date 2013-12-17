/*
 * =========================================================================
 *
 *       Filename:  UnxDmnSocketAddress.h
 *
 *    Description:  Unix域套接字地址封装
 *
 *        Version:  1.0
 *        Created:  2012-01-12 19:42:29
 *  Last Modified:  2012-01-12 19:42:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#ifndef UNXDMNSOCKETADDRESS_H_
#define UNXDMNSOCKETADDRESS_H_

#include <string>
#include <cstring>
#include <cstdlib>

#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>

/**
 * @brief Unix域套接字地址
 */
class UnxDmnSocketAddress
{
public:
    UnxDmnSocketAddress() :
        m_cstrPath(m_strPath),
        m_cSockAddr(m_SockAddr)
    {
        memset(&m_SockAddr, 0, sizeof(m_SockAddr));
        m_SockAddr.sun_family = AF_LOCAL;
    }

    explicit UnxDmnSocketAddress(const std::string &path) :
        m_cstrPath(m_strPath),
        m_cSockAddr(m_SockAddr)
    {
        m_strPath = path;
        memset(&m_SockAddr, 0, sizeof(m_SockAddr));
        m_SockAddr.sun_family = AF_LOCAL;
        memcpy(m_SockAddr.sun_path, m_strPath.c_str(), m_strPath.length());
    }

    UnxDmnSocketAddress(const UnxDmnSocketAddress &addr) :
        m_cstrPath(m_strPath),
        m_cSockAddr(m_SockAddr)
    {
        m_strPath = addr.m_strPath;
        m_SockAddr = addr.m_SockAddr;
    }

    virtual ~UnxDmnSocketAddress()
    {

    }

    UnxDmnSocketAddress& operator=(const UnxDmnSocketAddress &addr)
    {
        m_strPath = addr.m_strPath;
        m_SockAddr = addr.m_SockAddr;

        return *this;
    }

    void setAddress(const std::string &path)
    {
        m_strPath = path;
        memcpy(m_SockAddr.sun_path, m_strPath.c_str(), m_strPath.length());
    }

    void setAddress(const struct sockaddr_un &addr)
    {
        m_SockAddr = addr;
        m_strPath = m_SockAddr.sun_path;
    }

public:
    const std::string &m_cstrPath;
    const struct sockaddr_un &m_cSockAddr;

private:
    std::string m_strPath;
    struct sockaddr_un m_SockAddr;
};

#endif  // UNXDMNSOCKETADDRESS_H_

