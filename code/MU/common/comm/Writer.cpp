/*
 * =========================================================================
 *
 *       Filename:  Writer.cpp
 *
 *    Description:  将Buffer中的数据写到目的地
 *
 *        Version:  1.0
 *        Created:  2012-01-16 00:15:49
 *  Last Modified:  2012-01-16 00:15:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#include "Writer.h"

#include <list>

#include <sys/uio.h>

#include "Buffer.h"
#include "log/log.h"

int
Writer::doWrite(std::list<Iov> *pSendIovList)
{
    // 构造struct iovec数组
    int iovcnt = pSendIovList->size();
    struct iovec *pIovec = new struct iovec[iovcnt];
    memset(pIovec, 0, sizeof(struct iovec) * iovcnt);

    int i = 0;
    for (std::list<Iov>::iterator it = pSendIovList->begin();
            it != pSendIovList->end(); ++it, ++i) {
        pIovec[i] = it->m_Iovec;
    }

    // 调用Writer的虚函数doWriteInternal发送数据
    int rt = 0;
    rt = doWriteInternal(pIovec, iovcnt);

    delete [] pIovec;

    // error ocurred
    if (rt < 0) {
        ERROR_LOG("In Writer::doWrite, Writer::doWriteInternal error");
        return -1;
    }

    // 标记已发送的数据块
    int writen = rt;
    for (std::list<Iov>::iterator it = pSendIovList->begin();
            it != pSendIovList->end(); ++it) {
        if (writen > it->m_Iovec.iov_len) {
            writen -= it->m_Iovec.iov_len;
            it->m_Iovec.iov_base = NULL;
            it->m_Iovec.iov_len = 0;
        } else{
            it->m_Iovec.iov_base = static_cast<char *>(it->m_Iovec.iov_base) + writen;
            it->m_Iovec.iov_len -= static_cast<size_t>(writen);
            break;
        }
    }

    return 0;
}

