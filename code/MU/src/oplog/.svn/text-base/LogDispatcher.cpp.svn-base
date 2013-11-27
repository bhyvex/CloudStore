/*
 * @file LogDispatcher.cpp
 * @brief Agent for write operation logs.
 *
 * @version 1.0
 * @date Mon Jul  9 21:39:30 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "LogDispatcher.h"

#include "comm/comm.h"
#include "log/log.h"

#include "oplog/LogEvent.h"
#include "oplog/LogThread.h"
#include "protocol/protocol.h"
#include "frame/MUMacros.h"
#include "state/RuleManager.h"
#include "state/BucketManager.h"

#include <assert.h>
#include <inttypes.h>

#include <list>

LogDispatcher::~LogDispatcher()
{
    m_WriteEpollEvent.unregisterRWEvents();

    m_pEpoll = NULL;

    m_pLogThread = NULL;
}

LogDispatcher::LogDispatcher(Epoll *pEpoll)
{
    assert(pEpoll);

    m_pEpoll = pEpoll;

    // set state
    m_iConnect = CONNECTED;

    // init EpollEvent
    m_WriteEpollEvent.setEpoll(m_pEpoll);

    m_WriteEpollEvent.setHandler(this);

    // init pipe
    int fd[2];
    int rt = pipe(fd);

    if (0 != rt) {
        DEBUG_LOG("Syscall Error: pipe. %s", strerror(errno));
    }

    m_QueueRD = fd[0];
    m_QueueWR = fd[1];

    setNonblock(m_QueueWR);

    // set fd to epoll
    m_WriteEpollEvent.setFd(m_QueueWR);
    m_WriteEpollEvent.registerWEvent();

    // close the write event
    m_WriteEpollEvent.closeWevent();

    // start log thread
    m_pLogThread = new LogThread();
    m_pLogThread->setQueueFd(m_QueueRD);
    m_pLogThread->start();
}


int
LogDispatcher::setNonblock(int fd)
{
    int val = 0;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        DEBUG_LOG("Syscall Error: fcntl. %s", strerror(errno));
        return val;
    }

    val |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, val) < 0) {
        DEBUG_LOG("Syscall Error: fcntl. %s", strerror(errno));
        return -1;
    }

    return 0;
}

void
LogDispatcher::sendLog(LogEvent &event)
{
    cstore::Log_Entry entry;
    entry.set_seq_nr(event.m_SeqNr);
    entry.set_uid(event.m_UserId);
    entry.set_token(event.m_Token);
    entry.set_op_code(event.m_OpCode);

    cstore::File_Meta *pMeta = NULL;
    cstore::File_Attr *pAttr = NULL;
    cstore::Block_Meta *pBlock = NULL;

    switch (event.m_OpCode) {

    case MU_OP_PUT_USER:
    case MU_OP_DEL_USER: {
            // do nothing
            break;
        }

    case MU_OP_PUT_DIR:
    case MU_OP_DEL_DIR:
    case MU_OP_DEL_FILE: {
            entry.set_path(event.m_Path);
            break;
        }

    case MU_OP_MOV_FILE:
    case MU_OP_MOV_DIR: {
            entry.set_src_path(event.m_SrcPath);
            entry.set_dest_path(event.m_DestPath);
            break;
        }

    case MU_OP_PUT_FILE: {
            entry.set_path(event.m_Path);
            pMeta = entry.mutable_file_meta();
            pAttr = pMeta->mutable_attr();
            pAttr->set_version(event.m_FileMeta.m_Attr.m_Version);
            pAttr->set_type(event.m_FileMeta.m_Attr.m_Type);
            pAttr->set_mode(event.m_FileMeta.m_Attr.m_Mode);
            pAttr->set_size(event.m_FileMeta.m_Attr.m_Size);
            pAttr->set_ctime(event.m_FileMeta.m_Attr.m_CTime);
            pAttr->set_mtime(event.m_FileMeta.m_Attr.m_MTime);

            for (std::list<BlockMeta>::const_iterator
                 it = event.m_FileMeta.m_BlockList.begin();
                 it != event.m_FileMeta.m_BlockList.end(); ++it) {
                pBlock = pMeta->add_block_list();
                pBlock->set_checksum(it->m_Checksum);
            }

            break;
        }

    default: {
            DEBUG_LOG("unexpected log op code %d", event.m_OpCode);

            assert(0);
            return ;
        }

    }

    if (!entry.SerializeToString(&(event.m_SerializedLog))) {
        DEBUG_LOG("protobuf serialize failed");
        return ;
    }

    LogThreadRequest *pReq = new LogThreadRequest();
    pReq->m_OpType = LogThreadRequest::WRITE_LOG;
    pReq->m_LogEvent = event;
    m_ReqList.push_back(pReq);

    // 打开线程池句柄写事件
    m_WriteEpollEvent.openWevent();
}

void
LogDispatcher::rotateLog(uint64_t bucketId)
{
    Bucket *pBucket = BucketManager::getInstance()->get(bucketId);

    if (NULL == pBucket) {
        FATAL_LOG("try to rotate log for bucket %" PRIu64
                  ", but it does not exist", bucketId);
        return ;
    }

    LogEvent event;

    event.m_BucketId = bucketId;
    event.m_UserId = UINT64_MAX;
    event.m_OpCode = MU_OP_ROTATE_LOG;
    event.m_SeqNr = ++(pBucket->m_LogSeq);

    cstore::Log_Entry entry;
    entry.set_seq_nr(event.m_SeqNr);
    entry.set_uid(event.m_UserId);
    entry.set_token(event.m_Token);
    entry.set_op_code(event.m_OpCode);

    if (!entry.SerializeToString(&(event.m_SerializedLog))) {
        DEBUG_LOG("protobuf serialize failed");
        FATAL_LOG("error occurred when we "
                  "try to rotate log for bucket %" PRIu64,
                  bucketId);
        return ;
    }

    LogThreadRequest *pReq = new LogThreadRequest();
    pReq->m_OpType = LogThreadRequest::ROTATE_LOG;
    pReq->m_LogEvent = event;
    pReq->m_TotalBuckets = RuleManager::getInstance()->getTotalBuckets();
    m_ReqList.push_back(pReq);

    // 打开线程池句柄写事件
    m_WriteEpollEvent.openWevent();
}

void
LogDispatcher::closeLog(uint64_t bucketId)
{
    LogThreadRequest *pReq = new LogThreadRequest();
    pReq->m_OpType = LogThreadRequest::CLOSE_LOG;
    pReq->m_LogEvent.m_BucketId = bucketId;
    m_ReqList.push_back(pReq);

    // 打开线程池句柄写事件
    m_WriteEpollEvent.openWevent();
}

void
LogDispatcher::flushLog(uint64_t bucketId)
{
    LogThreadRequest *pReq = new LogThreadRequest();
    pReq->m_OpType = LogThreadRequest::FLUSH_LOG;
    pReq->m_LogEvent.m_BucketId = bucketId;
    m_ReqList.push_back(pReq);

    // 打开线程池句柄写事件
    m_WriteEpollEvent.openWevent();
}

int
LogDispatcher::sendData()
{
    int rt = 0;

    while (!m_ReqList.empty()) {
        rt = sendRequestToLogThread(m_ReqList.front());

        if (0 != rt) {
            if (EAGAIN != rt) {
                DEBUG_LOG("Error occurred while post request to thread pool, "
                          "%s", strerror(errno));
                //return -1;
                break;
            }

            break;

        } else {
            m_ReqList.pop_front();
        }
    }

    if (m_ReqList.empty()) {
        // 关闭写事件
        m_WriteEpollEvent.closeWevent();
    }

    return 0;
}

int
LogDispatcher::recvData()
{
    return 0;
}

int
LogDispatcher::sendRequestToLogThread(LogThreadRequest *pReq)
{
    int rt = util::io::writen(m_QueueWR, &pReq, sizeof(void *));

    if (sizeof(void *) != rt) {
        if (EAGAIN != errno) {
            DEBUG_LOG("writen error");
        }

        return errno;
    }

    return 0;
}

