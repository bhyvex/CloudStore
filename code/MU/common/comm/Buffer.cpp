/*
 * =========================================================================
 *
 *       Filename:  Buffer.cpp
 *
 *    Description:  数据收发缓冲管理
 *
 *        Version:  1.0
 *        Created:  2012-01-16 18:32:11
 *  Last Modified:  2012-01-16 18:32:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#include "Buffer.h"

#include <list>

#include "log/log.h"

#include "Reader.h"
#include "Writer.h"
#include "ReadCallback.h"
#include "WriteCallback.h"
#include "AppProtoSpec.h"

Buffer::Buffer()
{
    m_pReader = NULL;
    m_pWriter = NULL;
    m_pReadCallback = NULL;
    m_pWriteCallback = NULL;
    m_pAppProtoSpec = NULL;

    m_nReadOffset = 0;
    m_nHeaderSize = 0;
    m_nContentLength = 0;
    m_bReadHeader = true;
    m_bNewPackage = true;

    m_pHeader = NULL;
    m_pContent = NULL;
}

Buffer::Buffer(Reader *pReader, Writer *pWriter,
               ReadCallback *pReadCallback, WriteCallback *pWriteCallback,
               AppProtoSpec *pAppProtoSpec)
{
    m_pReader = pReader;
    m_pWriter = pWriter;
    m_pReadCallback = pReadCallback;
    m_pWriteCallback = pWriteCallback;
    m_pAppProtoSpec = pAppProtoSpec;

    m_nReadOffset = 0;
    m_nHeaderSize = 0;
    m_nContentLength = 0;
    m_bReadHeader = true;
    m_bNewPackage = true;

    m_pHeader = NULL;
    m_pContent = NULL;
}

Buffer::~Buffer()
{
    releaseSendBuffer();

    delete [] m_pHeader;
    delete [] m_pContent;
    m_pHeader = NULL;
    m_pContent = NULL;

    delete m_pReader;
    delete m_pWriter;
    delete m_pReadCallback;
    delete m_pWriteCallback;
    delete m_pAppProtoSpec;
}

void
Buffer::releaseSendBuffer()
{
    for (std::list<Iov>::iterator it = m_SendIovList.begin();
         it != m_SendIovList.end();) {
        delete [] it->m_pCompleteBuffer;
        delete it->m_pContext;

        m_SendIovList.erase(it++);
    }
}

void
Buffer::writeToBuffer(char *pBuf, size_t len, DataContext *pContext)
{
    m_SendIovList.push_back(Iov(pBuf, len, pContext));
}

int
Buffer::write()
{
    if (NULL == m_pWriter) {
        ERROR_LOG("In Buffer::write, m_pWriter == NULL");
        return -1;
    }

    int rt = 0;

    // 发送数据
    rt = m_pWriter->doWrite(&m_SendIovList);

    if (rt < 0) {
        ERROR_LOG("In Buffer::write, m_pWriter->doWrite error");
        return -1;
    }

    // 回调

    // 将已发送的Iov拷贝到新list中，然后再依次回调。
    // 原因是在回调中可能以delete this的方式释放Agent，
    // 这样当前Buffer也已释放，不能再访问成员m_SendIovList
    std::list<Iov> cbIovList;

    for (std::list<Iov>::iterator it = m_SendIovList.begin();
         it != m_SendIovList.end();) {
        if (0 == it->m_Iovec.iov_len) {
            // 已发送完成
            cbIovList.push_back(*it);
            m_SendIovList.erase(it++);

        } else {
            // 第一个未完成缓冲区
            break;
        }
    }

    // 依次回调
    for (std::list<Iov>::iterator it = cbIovList.begin();
         it != cbIovList.end();) {
        if (NULL == m_pWriteCallback) {
            WARN_LOG("In Buffer::write, m_pWriteCallback == NULL");

        } else {
            rt = m_pWriteCallback->run(true, it->m_pContext);

            if (rt < 0) {
                ERROR_LOG("In Buffer::write, m_pWriteCallback->run error");
                return -1;
            }
        }

        delete [] it->m_pCompleteBuffer;
        delete it->m_pContext;

        cbIovList.erase(it++);
    }

    return 0;
}

int
Buffer::read()
{
    if (NULL == m_pReader) {
        ERROR_LOG("In Buffer::read, m_pReader == NULL");
        return -1;
    }

    if (NULL == m_pAppProtoSpec) {
        ERROR_LOG("In Buffer::read, m_pAppProtoSpec == NULL");
        return -1;
    }

    if (m_bNewPackage) {
        // 开始接收新的数据包
        delete [] m_pHeader;
        m_pHeader = NULL;
        delete [] m_pContent;
        m_pContent = NULL;

        m_nReadOffset = 0;
        m_nHeaderSize = 0;
        m_nContentLength = 0;

        m_nHeaderSize = m_pAppProtoSpec->getHeaderSize();
        m_pHeader = new char[m_nHeaderSize];

        m_bNewPackage = false;
        m_bReadHeader = true;
    }

    int rt = 0;

    if (m_bReadHeader) {
        // 读头
        rt =  readHeader();

        if (rt < 0) {
            return rt;
        }
    }

    if (!m_bNewPackage && !m_bReadHeader) {
        // 读负载
        rt = readContent();

        if (rt < 0) {
            return rt;
        }
    }

    return 0;
}

int
Buffer::readHeader()
{
    int rt = 0;

    rt = m_pReader->doRead(m_pHeader + m_nReadOffset,
                           m_nHeaderSize - m_nReadOffset);

    if (rt < 0 && (errno == EWOULDBLOCK || errno == EINTR)) {
        // 读阻塞或系统调用被中断
        return 0;

    } else if (rt < 0) {
        // error
        ERROR_LOG("In Buffer::readHeader, m_pReader->doRead error");
        return -1;

    } else if (0 == rt) {
        // connection closed by opposition
        WARN_LOG("In Buffer::readHeader, connection closed by opposition");
        return -1;
    }

    m_nReadOffset += rt;

    if (m_nReadOffset == m_nHeaderSize) {
        // 读头结束
        m_nReadOffset = 0;
        m_bReadHeader = false;

        // 取得负载数据长度
        m_nContentLength = m_pAppProtoSpec->getContentLength(
                               m_pHeader, m_nHeaderSize);

        if (0 == m_nContentLength) {
            // 无负载
            m_bNewPackage = true;

            if (NULL == m_pReadCallback) {
                WARN_LOG("In Buffer::readHeader, m_pReadCallback == NULL");
                return 0;
            }

            // callback
            rt = m_pReadCallback->run(m_pHeader, m_nHeaderSize,
                                      NULL, 0);

            if (rt < 0) {
                ERROR_LOG("In Buffer::readHeader, m_pReadCallback->run error");
                return -1;
            }

            return 0;

        } else if (m_nContentLength > 0) {
            // 有负载
            m_pContent = new char[m_nContentLength];
            return 0;

        } else {
            // error
            m_bNewPackage = true;
            ERROR_LOG("In Buffer::readHeader, "
                      "m_pAppProtoSpec->getContentLength() error");
            return -1;
        }
    }

    return 0;
}

int
Buffer::readContent()
{
    int rt = 0;

    rt = m_pReader->doRead(m_pContent + m_nReadOffset,
                           m_nContentLength - m_nReadOffset);

    if (rt < 0 && (errno == EWOULDBLOCK || errno == EINTR)) {
        // 读阻塞或系统调用被中断
        return 0;

    } else if (rt < 0) {
        // error
        ERROR_LOG("In Buffer::readContent, m_pReader->doRead error");
        return -1;

    } else if (0 == rt) {
        // connection closed by opposition
        WARN_LOG("In Buffer::readContent, connection closed by opposition");
        return -1;
    }

    m_nReadOffset += rt;

    if (m_nContentLength == m_nReadOffset) {
        // 读负载结束
        m_nReadOffset = 0;
        m_bNewPackage = true;

        // callback
        if (NULL == m_pReadCallback) {
            WARN_LOG("In Buffer::readContent, m_pReadCallback == NULL");
            return 0;
        }

        rt = m_pReadCallback->run(m_pHeader, m_nHeaderSize,
                                  m_pContent, m_nContentLength);

        if (rt < 0) {
            ERROR_LOG("In Buffer::readContent, m_pReadCallback->run error");
            return -1;
        }

        return 0;
    }

    return 0;
}

