/*
 * =========================================================================
 *
 *       Filename:  UnxDmnWriter.h
 *
 *    Description:  通过Unix域套接字发送数据
 *
 *        Version:  1.0
 *        Created:  2012-02-05 22:31:22
 *  Last Modified:  2012-02-05 22:31:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#ifndef UNXDMNWRITER_H_
#define UNXDMNWRITER_H_

#include "Writer.h"
#include "UnxDmnSocket.h"

class UnxDmnWriter : public Writer
{
public:
    explicit UnxDmnWriter(const UnxDmnSocket &socket) {
        m_Socket = socket;
    }

    virtual ~UnxDmnWriter() {

    }

protected:
    virtual int doWriteInternal(const struct iovec *pIovec, int iovcnt) {
        return m_Socket.writev(pIovec, iovcnt);
    }

private:
    UnxDmnSocket m_Socket;
};

#endif  // UNXDMNWRITER_H_

