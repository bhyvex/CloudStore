/*
 * =========================================================================
 *
 *       Filename:  Buffer.h
 *
 *    Description:  数据收发缓冲区管理
 *
 *        Version:  1.0
 *        Created:  2012-01-15 22:56:04
 *  Last Modified:  2012-01-15 22:56:04
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <list>
#include <cstring>

#include <sys/uio.h>
#include <sys/types.h>

//class Reader;
//class Writer;
//class ReadCallback;
//class WriteCallback;
//class AppProtoSpec;

#include "Reader.h"
#include "Writer.h"
#include "ReadCallback.h"
#include "WriteCallback.h"
#include "AppProtoSpec.h"

/**
 * @brief 数据上下文，如数据块来源等
 */
struct DataContext {
    DataContext() {

    }

    virtual ~DataContext() {

    }
};

/**
 * @brief 写出缓冲区数据块
 */
struct Iov {
    Iov() {
        m_pContext = NULL;
        m_pCompleteBuffer = NULL;
        memset(&m_Iovec, 0, sizeof(m_Iovec));
    }

    Iov(char *pBuf, size_t len, DataContext *pContext = NULL) {
        m_pCompleteBuffer = pBuf;
        m_Iovec.iov_base = pBuf;
        m_Iovec.iov_len = len;
        m_pContext = pContext;
    }

    virtual ~Iov() {
        m_Iovec.iov_base = NULL;
        m_Iovec.iov_len = 0;
        m_pCompleteBuffer = NULL;
        m_pContext = NULL;
    }

    // data
    struct iovec m_Iovec;
    // 完整的数据缓冲区，用于缓冲区释放
    // 发送部分数据后m_Iovec.iov_base将不再指向完整的缓冲区
    char *m_pCompleteBuffer;
    // 标识数据块所属上下文，如数据块来源
    // 指向的对象必须是new出来的，会在Buffer中被释放
    DataContext *m_pContext;
};

/**
 * @brief 数据收发缓冲区管理
 */
class Buffer
{
public:
    Buffer();
    Buffer(Reader *pReader, Writer *pWriter,
           ReadCallback *pReadCallback, WriteCallback *pWriteCallback,
           AppProtoSpec *pAppProtoSpec);
    virtual ~Buffer();

    /**
     * @brief 使用Reader接收数据
     *
     * @return
     */
    int read();

    /**
     * @brief 使用Writer发送数据
     *
     * @return
     */
    int write();

    /**
     * @brief 将欲发送数据写到Buffer中
     *
     * @param pBuf
     * @param len
     * @param pContext 指向的对象必须是new出来的
     */
    void writeToBuffer(char *pBuf, size_t len, DataContext *pContext = NULL);

    int getSendBufferLength() const {
        return m_SendIovList.size();
    }

    //
    // 以下set系列函数，参数必须是new出来的对象的指针，并且不用释放
    //
    void setReader(Reader *pReader) {
        if (NULL != m_pReader) {
            delete m_pReader;
        }

        m_pReader = pReader;
    }

    void setWriter(Writer *pWriter) {
        if (NULL != m_pWriter) {
            delete m_pWriter;
        }

        m_pWriter = pWriter;
    }

    void setReadCallback(ReadCallback *pReadCallback) {
        if (NULL != m_pReadCallback) {
            delete m_pReadCallback;
        }

        m_pReadCallback = pReadCallback;
    }

    void setWriteCallback(WriteCallback *pWriteCallback) {
        if (NULL != m_pWriteCallback) {
            delete m_pWriteCallback;
        }

        m_pWriteCallback = pWriteCallback;
    }

    void setAppProtoSpec(AppProtoSpec *pAppProtoSpec) {
        if (NULL != m_pAppProtoSpec) {
            delete m_pAppProtoSpec;
        }

        m_pAppProtoSpec = pAppProtoSpec;
    }

protected:
    void releaseSendBuffer();

    int readHeader();

    int readContent();

protected:
    // can't copy
    Buffer(const Buffer &buffer);
    Buffer &operator=(const Buffer &buffer);

private:
    // 发送缓冲区
    std::list<Iov> m_SendIovList;

    // 读缓冲
    int m_nReadOffset;
    int m_nHeaderSize;
    int m_nContentLength;
    char *m_pHeader;
    char *m_pContent;
    bool m_bReadHeader;
    bool m_bNewPackage;  // 是否将接收新的数据包

    Reader *m_pReader;
    Writer *m_pWriter;
    ReadCallback *m_pReadCallback;
    WriteCallback *m_pWriteCallback;
    AppProtoSpec *m_pAppProtoSpec;
};

#endif  // BUFFER_H_

