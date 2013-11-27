/*
 * =========================================================================
 *
 *       Filename:  Writer.h
 *
 *    Description:  将Buffer中的数据写到目的地
 *
 *        Version:  1.0
 *        Created:  2012-01-15 22:42:28
 *  Last Modified:  2012-01-15 22:42:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#ifndef WRITER_H_
#define WRITER_H_

#include <list>

#include <sys/uio.h>

struct Iov;

/**
 * @brief 将Buffer中的数据写到目的地
 */
class Writer
{
public:
    Writer() {

    }

    virtual ~Writer() {

    }

    /**
     * @brief 对每一个写出的数据块Iov，标识已写数据的偏移量
     *
     * @param pSendIovList 发送缓冲区
     *
     * @return  0 - success
     *         -1 - failed
     */
    virtual int doWrite(std::list<Iov> *pSendIovList);

protected:
    /**
     * @brief 以聚集写方式将数据写出
     *
     * @param pIovec
     * @param iovcnt
     *
     * @return 实际写出的字节数
     */
    virtual int doWriteInternal(const struct iovec *pIovec, int iovcnt) = 0;

protected:
    // can't copy
    Writer(const Writer &writer);
    Writer& operator=(const Writer &writer);
};

#endif  // WRITER_H_

