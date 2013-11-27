/*
 * =========================================================================
 *
 *       Filename:  UnxDmnReader.h
 *
 *    Description:  通过Unix域套接字接收数据
 *
 *        Version:  1.0
 *        Created:  2012-02-05 22:23:48
 *  Last Modified:  2012-02-05 22:23:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#ifndef UNXDMNREADER_H_
#define UNXDMNREADER_H_

#include "Reader.h"
#include "UnxDmnSocket.h"

class UnxDmnReader : public Reader
{
public:
    explicit UnxDmnReader(const UnxDmnSocket &socket) {
        m_Socket = socket;
    }

    virtual ~UnxDmnReader() {

    }

protected:
    virtual int doReadInternal(char *pBuf, size_t len) {
        return m_Socket.read(pBuf, len);
    }

private:
    UnxDmnSocket m_Socket;
};

#endif  // UNXDMNREADER_H_

