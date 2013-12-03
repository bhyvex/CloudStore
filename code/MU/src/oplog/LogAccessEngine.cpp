/*
 * @file LogAccessEngine.cpp
 * @brief You can do all log operations via this class.
 *
 * @version 1.0
 * @date Mon Jul  9 16:03:13 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "LogAccessEngine.h"

#include "Connection.h"
#include "Statement.h"
#include "LogEvent.h"

#include "frame/MUConfiguration.h"
#include "frame/MUMacros.h"
#include "frame/ReturnStatus.h"
#include "protocol/protocol.h"
#include "util/util.h"
#include "log/log.h"
#include "storage/ChannelManager.h"
#include "storage/Channel.h"
#include "storage/NameSpace.h"
#include "storage/FSNameSpace.h"

#include "protocol/MUMacros.h"


#include <stdio.h>
#include <time.h>

#include <map>

#define LOG_TABLE_NAME          "t_log_"

#define USER_TABLE_NAME         "t_user_"

#define MAX_USER_LOG_QUERY      "100"

LogAccessEngine::~LogAccessEngine()
{
    for (std::map<uint64_t, Connection *>::iterator it = m_ConnMap.begin();
         it != m_ConnMap.end();) {
        it->second->close();
        delete it->second;
        m_ConnMap.erase(it++);
    }

    for (std::map<uint64_t, RWLock *>::iterator it = m_LockMap.begin();
         it != m_LockMap.end();) {
        delete it->second;
        m_LockMap.erase(it++);
    }
}

LogAccessEngine::LogAccessEngine()
{

}

Connection *
LogAccessEngine::getConnection(uint64_t bucketId)
{
    Connection *pConn = NULL;

    std::map<uint64_t, Connection *>::iterator it = m_ConnMap.find(bucketId);

    if (m_ConnMap.end() == it) {
        pConn = createConnection(bucketId, true, true);

    } else {
        pConn = it->second;
    }

    if (NULL == pConn) {
        return pConn;
    }

    // already in transaction

    if (pConn->inTransaction()) {
        return pConn;
    }

    // open a transaction

    int rt = 0;

    rt = pConn->begin();

    if (-1 == rt) {
        WARN_LOG("begin transaction on log connection failed, bucket %"
                 PRIu64, bucketId);
    }

    return pConn;
}

Connection *
LogAccessEngine::createConnection(
    uint64_t bucketId,
    bool create,
    bool inMap
)
{
    std::string filePath = logPath(bucketId);

    int rt = 0;

    Connection *pConn = new Connection();

    if (create) {

        rt = pConn->open(filePath, true);

        if (-1 == rt) {
            ERROR_LOG("Connect to database failed, database file path %s.",
                      filePath.c_str());

            pConn->close();
            delete pConn;

            return NULL;
        }

        rt = createModel(pConn);

        if (-1 == rt) {
            ERROR_LOG("Create physical model failed, database file path %s.",
                      filePath.c_str());

            pConn->close();
            delete pConn;

            return NULL;
        }

    } else {
        rt = pConn->open(filePath, false);

        if (-1 == rt) {
            ERROR_LOG("connect to database failed, database file path %s",
                      filePath.c_str());

            pConn->close();
            delete pConn;

            return NULL;
        }
    }

    if (inMap) {
        m_ConnMap.insert(std::pair<uint64_t, Connection *>(bucketId, pConn));
    }

    return pConn;
}

std::string
LogAccessEngine::logPath(uint64_t bucketId)
{
	Channel* pInfoChannel = ChannelManager::getInstance()->findChannel(MUConfiguration::getInstance()->m_MainChannelID);
	NameSpace *InfoNS = pInfoChannel->m_DataNS;
	
    return (
               InfoNS->m_Root+
               PATH_SEPARATOR_STRING +
               BUCKET_NAME_PREFIX +
               util::conv::conv<std::string, uint64_t>(bucketId) +
               PATH_SEPARATOR_STRING +
               BUCKET_LOG_FILE_NAME
           );
}

std::string
LogAccessEngine::logPath(uint64_t bucketId, int logIdx)
{
    if (logIdx > 0) {
        return logPath(bucketId)
               + "."
               + util::conv::conv<std::string, int>(logIdx);

    } else {
        return logPath(bucketId);
    }
}

bool
LogAccessEngine::hasModel(Connection *pConn)
{
    Statement *pStmt = pConn->createStatement();

    StatementGuard stmtGuard(&pStmt);

    int rt = 0;

    std::string sql = "select name from sqlite_master "
                      "where type = 'table' and name = '"
                      LOG_TABLE_NAME "'";

    rt = pStmt->prepare(sql);

    if (-1 == rt) {
        DEBUG_LOG("prepare statement failed, %s", sql.c_str());
        return false;
    }

    rt = pStmt->execute();

    if (SQLITE_ROW == rt) {
        return true;
    }

    return false;
}

int
LogAccessEngine::createModel(Connection *pConn)
{
    int rt = 0;

    rt = createTable(pConn);

    if (-1 == rt) {
        DEBUG_LOG("create tables failed");
        return -1;
    }

    rt = createIndex(pConn);

    if (-1 == rt) {
        DEBUG_LOG("create indexes failed");
        return -1;
    }

    rt = createTrigger(pConn);

    if (-1 == rt) {
        DEBUG_LOG("create triggers failed");
        return -1;
    }

    return 0;
}

int
LogAccessEngine::createTable(Connection *pConn)
{
    int rt = 0;

    {

        Statement *pStmt = pConn->createStatement();

        StatementGuard stmtGuard(&pStmt);

        std::string sql =  "create table if not exists " LOG_TABLE_NAME " ("
                           "seq_ unsigned big int primary key, "
                           "uid_ unsigned big int, "
                           "token_ varchar(255), "
                           "log_ text"
                           ")";

        rt = pStmt->prepare(sql);

        if (-1 == rt) {
            DEBUG_LOG("prepare statement failed, %s", sql.c_str());
            return -1;
        }

        rt = pStmt->execute();

        if (-1 == rt) {
            DEBUG_LOG("create log table failed");
            return -1;
        }

    }

    {

        Statement *pStmt = pConn->createStatement();

        StatementGuard stmtGuard(&pStmt);

        std::string sql = "create table if not exists " USER_TABLE_NAME " ( "
                          "uid_ unsigned big int primary key, "
                          "seq_ unsigned big int"
                          ")";

        rt = pStmt->prepare(sql);

        if (-1 == rt) {
            DEBUG_LOG("prepare statement failed, %s", sql.c_str());
            return -1;
        }

        rt = pStmt->execute();

        if (-1 == rt) {
            DEBUG_LOG("create user table failed");
            return -1;
        }

    }

    return 0;
}

int
LogAccessEngine::createIndex(Connection *pConn)
{
    Statement *pStmt = pConn->createStatement();

    StatementGuard stmtGuard(&pStmt);

    int rt = 0;

    std::string sql = "create index if not exists idx_log_uid_ on "
                      LOG_TABLE_NAME
                      " (uid_)";

    rt = pStmt->prepare(sql);

    if (-1 == rt) {
        DEBUG_LOG("prepare statement failed, %s", sql.c_str());
        return -1;
    }

    rt = pStmt->execute();

    if (-1 == rt) {
        DEBUG_LOG("create index failed");
        return -1;
    }

    return 0;
}

int
LogAccessEngine::createTrigger(Connection *pConn)
{
    Statement *pStmt = pConn->createStatement();

    StatementGuard stmtGuard(&pStmt);

    int rt = 0;

    std::string sql = "create trigger if not exists tgr_log_insert "
                      "after insert on " LOG_TABLE_NAME
                      " for each row "
                      " begin "
                      "update " USER_TABLE_NAME " set seq_ = new.seq_ "
                      "where uid_ = new.uid_; "
                      "end";

    rt = pStmt->prepare(sql);

    if (-1 == rt) {
        DEBUG_LOG("prepare statement failed, %s", sql.c_str());
        return -1;
    }

    rt = pStmt->execute();

    if (-1 == rt) {
        DEBUG_LOG("create trigger failed");
        return -1;
    }

    return 0;
}

int
LogAccessEngine::write(const LogEvent &event)
{
    RWLockGuard lockGuard(
        getBucketLock(event.m_BucketId),
        RWLockGuard::WRITE
    );

    return writeLogLocked(event);
}

int
LogAccessEngine::rotate(
    uint64_t bucketId,
    uint64_t totalBuckets,
    const LogEvent &event
)
{
    RWLockGuard lockGuard(getBucketLock(bucketId), RWLockGuard::WRITE);

    int rt = 0;

    int logFiles = MUConfiguration::getInstance()->m_RotateLogFiles;

    std::string bucketRoot = bucketRootPath(bucketId);

    std::string src;
    std::string dest;
    std::string base;

    base = bucketRoot + PATH_SEPARATOR_STRING + BUCKET_LOG_FILE_NAME;

    src = base + "." + util::conv::conv<std::string, int>(logFiles);
    dest = src + "." + util::conv::conv<std::string, time_t>(time(NULL));

    TRACE_LOG("rotate log, src %s, dest %s",
              src.c_str(), dest.c_str());

    if (isfile(src)) {
        rt = ::rename(src.c_str(), dest.c_str());

        if (-1 == rt) {
            DEBUG_LOG("rename() error, %s.", strerror(errno));
            return -1;
        }
    }


    for (int i = logFiles - 1; i > 0; --i) {
        src = base + "." + util::conv::conv<std::string, int>(i);
        dest = base + "." + util::conv::conv<std::string, int>(i + 1);

        TRACE_LOG("rotate log, src %s, dest %s",
                  src.c_str(), dest.c_str());

        if (isfile(src)) {
            rt = ::rename(src.c_str(), dest.c_str());

            if (-1 == rt) {
                DEBUG_LOG("rename() error, %s.", strerror(errno));
                return -1;
            }
        }
    }

    src = base;
    dest = base + ".1";

    TRACE_LOG("rotate log, src %s, dest %s",
              src.c_str(), dest.c_str());

    rt = ::rename(src.c_str(), dest.c_str());

    if (-1 == rt) {
        DEBUG_LOG("rename() error, %s.", strerror(errno));
        return -1;
    }

    // close old connection
    closeConnection(bucketId);

    // copy user table
    rt = copyUserTable(bucketId, totalBuckets);

    if (-1 == rt) {
        ERROR_LOG("copy user table failed, bucket %" PRIu64, bucketId);
        return -1;
    }

    // log the rotation operation

    rt = writeLogLocked(event);

    if (-1 == rt) {
        ERROR_LOG("trt to log the rotation operation failed, bucket %" PRIu64,
                  bucketId);
        return -1;
    }

    return 0;
}

ReturnStatus
LogAccessEngine::readUserLogFromCurrentLog(
    uint64_t bucketId, uint64_t userId,
    uint64_t seq,
    std::list<LogEvent> *pLogList,
    bool *pMore
)
{
    int rt = 0;

    //Connection *pConn = createConnection(bucketId, false, false);
    Connection *pConn = getConnection(bucketId);

    if (NULL == pConn) {
        DEBUG_LOG("Cannot get database connection, bucket id %" PRIu64,
                  bucketId);

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    //ConnectionGuard connGuard(&pConn);

    Statement *pStmt = pConn->createStatement();

    StatementGuard stmtGuard(&pStmt);

    std::string sql = "select seq_, log_ from " LOG_TABLE_NAME
                      " where seq_ >= ? and uid_ = ? order by seq_ asc"
                      " limit " MAX_USER_LOG_QUERY;

    rt = pStmt->prepare(sql);

    if (-1 == rt) {
        DEBUG_LOG("prepare statement failed, %s", sql.c_str());
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = pStmt->bindInt64(1, seq);

    if (-1 == rt) {
        DEBUG_LOG("bind error, column index %d", 1);
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = pStmt->bindInt64(2, userId);

    if (-1 == rt) {
        DEBUG_LOG("bind error, column index %d", 2);
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    LogEvent event;
    const char *pVal = NULL;
    int valLen = 0;

    *pMore = false;

    // fetch the first row in result set?
    bool firstRow = true;

    while (SQLITE_ROW == (rt = pStmt->step())) {
        if (firstRow) {
            // check the first row to determine if seq is outdated

            firstRow = false;

            event.m_SeqNr = pStmt->columnInt64(0);

            if (seq == event.m_SeqNr) {
                continue;

            } else {
                return ReturnStatus(MU_FAILED, LOG_SEQ_OUTDATED);
            }
        }

        pVal = pStmt->columnText(1);
        valLen = pStmt->columnBytes(1);
        event.m_SerializedLog = std::string(pVal, valLen);

        pLogList->push_back(event);

        if (pLogList->size() == MAX_LOG_RECORDS_ONCE) {
            // too many logs
            *pMore = true;
            return ReturnStatus(MU_SUCCESS);
        }
    }

    if (-1 == rt) {
        DEBUG_LOG("step error");
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    if (firstRow) {
        // no user logs in current log table

        ReturnStatus rs;
        uint64_t seqNr = 0;

        rs = queryCurrentUserLogSeqNr(bucketId, userId, &seqNr);

        if (!rs.success()) {
            DEBUG_LOG("query current user log seq nr failed, "
                      "bucket %" PRIu64 ", user %" PRIu64,
                      bucketId, userId);
            return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        }

        if (seqNr == seq) {
            // no new operations for current user
            return ReturnStatus(MU_SUCCESS);
        }

        // no logs in current log table,
        // but some new operations have been done,
        // this may be caused by log rotation

        //DEBUG_LOG("bucket id %" PRIu64 ", user id %" PRIu64
        //", seq nr %" PRIu64 ", "
        //"log seq outdated", bucketId, userId, seq);
        return ReturnStatus(MU_FAILED, LOG_SEQ_OUTDATED);
    }

    return ReturnStatus(MU_SUCCESS);
}


std::string
LogAccessEngine::bucketRootPath(uint64_t bucketId)
{
	Channel* pInfoChannel = ChannelManager::getInstance()->findChannel(MUConfiguration::getInstance()->m_MainChannelID);
	NameSpace *InfoNS = pInfoChannel->m_DataNS;
	
    return (
               InfoNS->m_Root +
               PATH_SEPARATOR_STRING +
               BUCKET_NAME_PREFIX +
               util::conv::conv<std::string, uint64_t>(bucketId)
           );
}

bool
LogAccessEngine::isfile(std::string &path)
{
	Channel* pInfoChannel = ChannelManager::getInstance()->findChannel(MUConfiguration::getInstance()->m_MainChannelID);
	NameSpace *InfoNS = pInfoChannel->m_DataNS;
	
    int rt = 0;
    FileAttr st;

    rt = InfoNS->Stat(path.c_str(), &st);

    if (-1 == rt) {
        DEBUG_LOG("stat() error, %s.", strerror(errno));
        return false;
    }

    if (MU_REGULAR_FILE == st.m_Type) {
        return true;
    }

    return false;
}

ReturnStatus
LogAccessEngine::queryCurrentBucketLogSeqNr(
    uint64_t bucketId, uint64_t *pSeqNr)
{
    RWLockGuard lockGuard(getBucketLock(bucketId), RWLockGuard::READ);

    int rt = 0;

    //Connection *pConn = createConnection(bucketId, false, false);
    Connection *pConn = getConnection(bucketId);

    if (NULL == pConn) {
        DEBUG_LOG("Cannot get database connection, bucket id %" PRIu64,
                  bucketId);

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    //ConnectionGuard connGuard(&pConn);

    Statement *pStmt = pConn->createStatement();

    StatementGuard stmtGuard(&pStmt);

    std::string sql = "select max(seq_) from " LOG_TABLE_NAME;

    rt = pStmt->prepare(sql);

    if (-1 == rt) {
        DEBUG_LOG("prepare statement failed, %s", sql.c_str());

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = pStmt->execute();

    if (-1 == rt) {
        DEBUG_LOG("execute prepared sql failed.");

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    if (SQLITE_ROW == rt) {
        *pSeqNr = pStmt->columnInt64(0);

    } else {
        return queryCurrentBucketLogSeqNrFromUserTable(bucketId, pSeqNr);
    }

    // actually sqlite3 will still return SQLITE_ROW
    // when no records in log table
    if (0 == *pSeqNr) {
        return queryCurrentBucketLogSeqNrFromUserTable(bucketId, pSeqNr);
    }

    return ReturnStatus(MU_SUCCESS);
}

void
LogAccessEngine::closeConnection(uint64_t bucketId)
{
    std::map<uint64_t, Connection *>::iterator it = m_ConnMap.find(bucketId);

    if (m_ConnMap.end() == it) {
        // no open connections to log file

    } else {
        it->second->close();
        delete it->second;

        m_ConnMap.erase(it);
    }
}

int
LogAccessEngine::close(uint64_t bucketId)
{
    closeConnection(bucketId);

    // destroy related lock

    std::map<uint64_t, RWLock *>::iterator it = m_LockMap.find(bucketId);

    if (m_LockMap.end() != it) {
        // lock and unlock, to ensure no one keep this lock
        it->second->wlock();
        it->second->unlock();

        delete it->second;
        m_LockMap.erase(it);
    }

    return 0;
}

ReturnStatus
LogAccessEngine::queryCurrentUserLogSeqNr(
    uint64_t bucketId, uint64_t userId, uint64_t *pSeqNr)
{
    RWLockGuard lockGuard(getBucketLock(bucketId), RWLockGuard::READ);

    int rt = 0;

    Connection *pConn = getConnection(bucketId);
    //Connection *pConn = createConnection(bucketId, false, false);

    if (NULL == pConn) {
        DEBUG_LOG("Cannot get database connection, bucket id %ld.",
                  bucketId);

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    //ConnectionGuard connGuard(&pConn);

    Statement *pStmt = pConn->createStatement();

    StatementGuard stmtGuard(&pStmt);

    std::string sql = "select seq_ from " USER_TABLE_NAME
                      " where uid_ = ?";

    rt = pStmt->prepare(sql);

    if (-1 == rt) {
        DEBUG_LOG("prepare statement failed, %s", sql.c_str());

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = pStmt->bindInt64(1, userId);

    if (-1 == rt) {
        DEBUG_LOG("bind parameter failed, index 1");
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = pStmt->execute();

    if (-1 == rt) {
        DEBUG_LOG("execute prepared sql failed.");

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    if (SQLITE_ROW == rt) {
        *pSeqNr = pStmt->columnInt64(0);

    } else {
        *pSeqNr = 0;
    }

    TRACE_LOG("current user log seq nr %" PRIu64, *pSeqNr);

    return ReturnStatus(MU_SUCCESS);
}

int
LogAccessEngine::createUser(uint64_t bucketId, uint64_t userId)
{
    RWLockGuard lockGuard(getBucketLock(bucketId), RWLockGuard::WRITE);

    int rt = 0;

    //Connection *pConn = createConnection(bucketId, false, false);
    Connection *pConn = getConnection(bucketId);

    if (NULL == pConn) {
        DEBUG_LOG("Cannot get database connection, bucket id %" PRIu64,
                  bucketId);
        return -1;
    }

    //ConnectionGuard connGuard(&pConn);

    Statement *pStmt = pConn->createStatement();

    StatementGuard stmtGuard(&pStmt);

    std::string sql = "insert into " USER_TABLE_NAME " (uid_, seq_) "
                      " values (?, ?)";

    rt = pStmt->prepare(sql);

    if (-1 == rt) {
        DEBUG_LOG("prepare statement failed, %s", sql.c_str());
        return -1;
    }

    rt = pStmt->bindInt64(1, userId);

    if (-1 == rt) {
        DEBUG_LOG("bind parameter failed, index 1");
        return -1;
    }

    rt = pStmt->bindInt64(2, 0);

    if (-1 == rt) {
        DEBUG_LOG("bind parameter failed, index 2");
        return -1;
    }

    rt = pStmt->execute();

    if (-1 == rt) {
        DEBUG_LOG("execute prepared sql failed.");
        return -1;
    }

    //rt = flushLocked(bucketId);

    //if (-1 == rt) {
        //DEBUG_LOG("flush logs failed when create user %" PRIu64, userId);
        //return -1;
    //}

    return 0;
}

int
LogAccessEngine::deleteUser(uint64_t bucketId, uint64_t userId)
{
    RWLockGuard lockGuard(getBucketLock(bucketId), RWLockGuard::WRITE);

    int rt = 0;

    //Connection *pConn = createConnection(bucketId, false, false);
    Connection *pConn = getConnection(bucketId);

    if (NULL == pConn) {
        DEBUG_LOG("Cannot get database connection, bucket id %ld.",
                  bucketId);
        return -1;
    }

    //ConnectionGuard connGuard(&pConn);

    Statement *pStmt = pConn->createStatement();

    StatementGuard stmtGuard(&pStmt);

    std::string sql = "delete from " USER_TABLE_NAME
                      " where uid_ = ?";

    rt = pStmt->prepare(sql);

    if (-1 == rt) {
        DEBUG_LOG("prepare statement failed, %s", sql.c_str());
        return -1;
    }

    rt = pStmt->bindInt64(1, userId);

    if (-1 == rt) {
        DEBUG_LOG("bind parameter failed, index 1");
        return -1;
    }

    rt = pStmt->execute();

    if (-1 == rt) {
        DEBUG_LOG("execute prepared sql failed");
        return -1;
    }

    return 0;
}

ReturnStatus
LogAccessEngine::queryCurrentBucketLogSeqNrFromUserTable(
    uint64_t bucketId,
    uint64_t *pSeqNr
)
{
    RWLockGuard lockGuard(getBucketLock(bucketId), RWLockGuard::READ);

    int rt = 0;

    //Connection *pConn = createConnection(bucketId, false, false);
    Connection *pConn = getConnection(bucketId);

    if (NULL == pConn) {
        DEBUG_LOG("Cannot get database connection, bucket id %" PRIu64,
                  bucketId);

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    //ConnectionGuard connGuard(&pConn);

    Statement *pStmt = pConn->createStatement();

    StatementGuard stmtGuard(&pStmt);

    std::string sql = "select max(seq_) from " USER_TABLE_NAME;

    rt = pStmt->prepare(sql);

    if (-1 == rt) {
        DEBUG_LOG("prepare statement failed, %s", sql.c_str());

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = pStmt->execute();

    if (-1 == rt) {
        DEBUG_LOG("execute prepared sql failed.");

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    if (SQLITE_ROW == rt) {
        *pSeqNr = pStmt->columnInt64(0);

    } else {
        *pSeqNr = 0;
    }

    return ReturnStatus(MU_SUCCESS);
}

RWLock *
LogAccessEngine::getBucketLock(uint64_t bucketId)
{
    // DCL

    std::map<uint64_t, RWLock *>::iterator it = m_LockMap.find(bucketId);

    if (m_LockMap.end() == it) {
        LockGuard lockGuard(&m_LockMapMutex);

        std::map<uint64_t, RWLock *>::iterator nit = m_LockMap.find(bucketId);

        if (m_LockMap.end() == nit) {
            RWLock *pLock = new RWLock();
            m_LockMap.insert(std::pair<uint64_t, RWLock *>(bucketId, pLock));

            return pLock;

        } else {
            return nit->second;
        }

    } else {
        return it->second;
    }
}

Connection *
LogAccessEngine::createConnectionToArchivedLog(
    uint64_t bucketId,
    int logIdx
)
{
    std::string filePath = logPath(bucketId, logIdx);

    int rt = 0;

    Connection *pConn = new Connection();

    rt = pConn->open(filePath, false);

    if (-1 == rt) {
        ERROR_LOG("connect to database failed, database file path %s",
                  filePath.c_str());

        pConn->close();
        delete pConn;

        return NULL;
    }

    return pConn;
}

int
LogAccessEngine::copyUserTable(
    uint64_t bucketId,
    uint64_t totalBuckets
)
{
    int rt = 0;

    Connection *pDestConn = getConnection(bucketId);

    if (NULL == pDestConn) {
        DEBUG_LOG("Cannot get database connection, bucket id %" PRIu64,
                  bucketId);
        return -1;
    }

    Connection *pSrcConn = createConnectionToArchivedLog(bucketId, 1);

    if (NULL == pSrcConn) {
        DEBUG_LOG("Cannot get database connection, bucket id %" PRIu64,
                  bucketId);
        return -1;
    }

    ConnectionGuard connGuard(&pSrcConn);

    std::string srcSql = "select uid_, seq_ from " USER_TABLE_NAME;
    std::string destSql = "insert into " USER_TABLE_NAME " (uid_, seq_) "
                          " values (?, ?)";

    Statement *pDestStmt = pDestConn->createStatement();
    StatementGuard destStmtGuard(&pDestStmt);

    Statement *pSrcStmt = pSrcConn->createStatement();
    StatementGuard srcStmtGuard(&pSrcStmt);

    rt = pDestStmt->prepare(destSql);

    if (-1 == rt) {
        DEBUG_LOG("prepare sql failed, %s", destSql.c_str());
        return -1;
    }

    rt = pSrcStmt->prepare(srcSql);

    if (-1 == rt) {
        DEBUG_LOG("prepare sql failed, %s", srcSql.c_str());
        return -1;
    }

    uint64_t uid = 0;
    uint64_t seq = 0;

    int nrt = 0;

    while (SQLITE_ROW == (rt = (pSrcStmt->step()))) {
        uid = pSrcStmt->columnInt64(0);
        seq = pSrcStmt->columnInt64(1);

        if (uid % totalBuckets != bucketId) {
            // do not belong to this bucket anymore
            continue;
        }

        nrt = pDestStmt->bindInt64(1, uid);

        if (-1 == rt) {
            DEBUG_LOG("bind parameter failed, index 1");
            return -1;
        }

        nrt = pDestStmt->bindInt64(2, seq);

        if (-1 == rt) {
            DEBUG_LOG("bind parameter failed, index 2");
            return -1;
        }

        nrt = pDestStmt->execute();

        if (-1 == rt) {
            DEBUG_LOG("execute prepared sql failed");
            return -1;
        }

        nrt = pDestStmt->clean();

        if (-1 == rt) {
            DEBUG_LOG("clean statement failed");
            return -1;
        }
    }

    if (-1 == rt) {
        DEBUG_LOG("step error");
        return -1;
    }

    return 0;
}

ReturnStatus
LogAccessEngine::createLogFile(uint64_t bucketId)
{
    RWLockGuard lockGuard(getBucketLock(bucketId), RWLockGuard::WRITE);

    Connection *pConn = createConnection(bucketId, true, false);

    ConnectionGuard connGuard(&pConn);
}

ReturnStatus
LogAccessEngine::readUserLogFromArchivedLog(
    uint64_t bucketId,
    uint64_t userId,
    uint64_t seq,
    int logIdx,
    std::list<LogEvent> *pLogList,
    bool *pMore
)
{
    int rt = 0;

    std::string logFile = logPath(bucketId, logIdx);

    if (!isfile(logFile)) {
        // log file not exists
        DEBUG_LOG("check log file %s failed", logFile.c_str());
        //return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        return ReturnStatus(MU_FAILED, LOG_SEQ_OUTDATED);
    }

    Connection *pConn = createConnectionToArchivedLog(bucketId, logIdx);

    if (NULL == pConn) {
        DEBUG_LOG("Cannot get database connection, bucket id %" PRIu64,
                  bucketId);

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    ConnectionGuard connGuard(&pConn);

    Statement *pStmt = pConn->createStatement();

    StatementGuard stmtGuard(&pStmt);

    std::string sql = "select seq_, log_ from " LOG_TABLE_NAME
                      " where seq_ > ? and uid_ = ? order by seq_ asc"
                      " limit " MAX_USER_LOG_QUERY;

    rt = pStmt->prepare(sql);

    if (-1 == rt) {
        DEBUG_LOG("prepare statement failed, %s", sql.c_str());
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = pStmt->bindInt64(1, seq);

    if (-1 == rt) {
        DEBUG_LOG("bind error, column index %d", 1);
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = pStmt->bindInt64(2, userId);

    if (-1 == rt) {
        DEBUG_LOG("bind error, column index %d", 2);
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    LogEvent event;
    const char *pVal = NULL;
    int valLen = 0;

    *pMore = false;

    while (SQLITE_ROW == (rt = pStmt->step())) {
        pVal = pStmt->columnText(1);
        valLen = pStmt->columnBytes(1);
        event.m_SerializedLog = std::string(pVal, valLen);

        pLogList->push_back(event);

        if (pLogList->size() == MAX_LOG_RECORDS_ONCE) {
            // too many logs
            *pMore = true;
            return ReturnStatus(MU_SUCCESS);
        }
    }

    if (-1 == rt) {
        DEBUG_LOG("step error");
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
LogAccessEngine::readUserLog(
    uint64_t bucketId,
    uint64_t userId,
    uint64_t seq,
    std::list<LogEvent> *pLogList,
    bool *pMore
)
{
    RWLockGuard lockGuard(getBucketLock(bucketId), RWLockGuard::READ);

    int rt = 0;
    ReturnStatus rs;

    // query from current log file

    rs = readUserLogFromCurrentLog(
             bucketId,
             userId,
             seq,
             pLogList,
             pMore);

    if (rs.success()
        || ((!rs.success())
            && (rs.errorCode != LOG_SEQ_OUTDATED))) {
        return rs;
    }

    TRACE_LOG("no logs in current log file, bucket %" PRIu64 ", "
              "user %" PRIu64, bucketId, userId);

    // query from archived log files

    int logIdx = 1;
    bool exist = false;

    int logs = MUConfiguration::getInstance()->m_RotateLogFiles;

    // which log file has log record 'seq'

    for (logIdx = 1; logIdx <= logs; ++logIdx) {
        TRACE_LOG("try to find log %" PRIu64
                  " in archived log file %" PRIi32 ", "
                  "bucket %" PRIu64 ", user %" PRIu64,
                  seq, logIdx, bucketId, userId);

        rs = userLogExistInArchiveLog(
                 bucketId,
                 userId,
                 seq,
                 logIdx,
                 &exist
             );

        if (!rs.success()) {
            // error
            return rs;
        }

        if (exist) {
            break;
        }
    }

    if (logIdx > logs) {
        // cannot find log record 'seq' in all log files
        return ReturnStatus(MU_FAILED, LOG_SEQ_OUTDATED);
    }

    // found log record 'seq' in log file 'logIdx'

    TRACE_LOG("found log %" PRIu64 " in log file %" PRIi32 ", "
              "bucket %" PRIu64 ", user %" PRIu64,
              seq, logIdx, bucketId, userId);

    for (int i = logIdx; i >= 1; --i) {
        TRACE_LOG("try to query logs in archived log file %" PRIi32 ", "
                  "bucket %" PRIu64 ", user %" PRIu64,
                  i, bucketId, userId);

        pLogList->clear();

        rs = readUserLogFromArchivedLog(
                 bucketId,
                 userId,
                 seq,
                 i,
                 pLogList,
                 pMore
             );

        if (rs.success() && pLogList->size() == 0) {
            // no current user's logs in this log file,
            // try to query from newer archive log
            continue;
        }

        if (rs.success()) {
            // get some logs in archived log file,
            // we have to set MORE_DATA flag to make client
            // request user logs once more.
            *pMore = true;
        }

        return rs;
    }

    // query in current log file

    TRACE_LOG("query logs in current log file again, "
              "bucket %" PRIu64 ", user %" PRIu64,
              bucketId, userId);

    rs = readUserLogFromArchivedLog(
             bucketId,
             userId,
             seq,
             0,
             pLogList,
             pMore
         );

    return rs;

    // should not reach here
    //return ReturnStatus(MU_FAILED, LOG_SEQ_OUTDATED);
}

ReturnStatus
LogAccessEngine::userLogExistInArchiveLog(
    uint64_t bucketId,
    uint64_t userId,
    uint64_t seq,
    int logIdx,
    bool *pExist
)
{
    int rt = 0;

    std::string logFile = logPath(bucketId, logIdx);

    if (!isfile(logFile)) {
        // log file not exists
        DEBUG_LOG("check log file %s failed", logFile.c_str());
        return ReturnStatus(MU_FAILED, LOG_SEQ_OUTDATED);
    }

    Connection *pConn = createConnectionToArchivedLog(bucketId, logIdx);

    if (NULL == pConn) {
        DEBUG_LOG("Cannot get database connection, bucket id %" PRIu64,
                  bucketId);

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    ConnectionGuard connGuard(&pConn);

    Statement *pStmt = pConn->createStatement();

    StatementGuard stmtGuard(&pStmt);

    std::string sql = "select seq_, log_ from " LOG_TABLE_NAME
                      " where seq_ = ? and uid_ = ?";

    rt = pStmt->prepare(sql);

    if (-1 == rt) {
        DEBUG_LOG("prepare statement failed, %s", sql.c_str());
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = pStmt->bindInt64(1, seq);

    if (-1 == rt) {
        DEBUG_LOG("bind error, column index %d", 1);
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = pStmt->bindInt64(2, userId);

    if (-1 == rt) {
        DEBUG_LOG("bind error, column index %d", 2);
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = pStmt->step();

    if (-1 == rt) {
        DEBUG_LOG("step error");
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    if (SQLITE_ROW == rt) {
        *pExist = true;

    } else {
        *pExist = false;
    }

    if (!(*pExist)) {
        // no user log 'seq' in this archived log file,
        // but 'seq' maybe the max seq nr before this archived log file
        // being archived

        ReturnStatus rs;
        uint64_t seqNr = 0;

        rs = queryUserLogSeqNrFromArchivedLog(
                 bucketId, userId, logIdx, &seqNr);

        if (!rs.success()) {
            DEBUG_LOG("query max user log seq nr "
                      "from archived log file %" PRIi32
                      " failed, bucket %" PRIu64 " user %" PRIu64,
                      logIdx, bucketId, userId);
            return rs;
        }

        if (seq == seqNr) {
            *pExist = true;
            TRACE_LOG("found max user log seq nr %" PRIu64
                      " in archived log file %" PRIi32,
                      seq, logIdx);

        } else {
            TRACE_LOG("max user log seq nr in log file %" PRIi32
                      " is %" PRIu64, logIdx, seqNr);
        }
    }

    return ReturnStatus(MU_SUCCESS);
}

int
LogAccessEngine::writeLogLocked(const LogEvent &event)
{
    int rt = 0;

    Connection *pConn = getConnection(event.m_BucketId);

    if (NULL == pConn) {
        DEBUG_LOG("Write log failed, can not connect to database, "
                  "bucket Id %" PRIu64, event.m_BucketId);
        return -1;
    }

    Statement *pStmt = pConn->createStatement();

    StatementGuard stmtGuard(&pStmt);

    std::string sql = "insert into " LOG_TABLE_NAME
                      " (seq_, uid_, token_, log_) values ("
                      "?, ?, ?, ?"
                      ")";

    rt = pStmt->prepare(sql);

    if (-1 == rt) {
        DEBUG_LOG("prepare statement failed, %s", sql.c_str());
        return -1;
    }

    rt = pStmt->bindInt64(1, event.m_SeqNr);

    if (-1 == rt) {
        DEBUG_LOG("bind error, column index %d", 1);
        return -1;
    }

    rt = pStmt->bindInt64(2, event.m_UserId);

    if (-1 == rt) {
        DEBUG_LOG("bind error, column index %d", 2);
        return -1;
    }

    rt = pStmt->bindText(3, event.m_Token);

    if (-1 == rt) {
        DEBUG_LOG("bind error, column index %d", 3);
        return -1;
    }

    rt = pStmt->bindText(4, event.m_SerializedLog);

    if (-1 == rt) {
        DEBUG_LOG("bind error, column index %d", 4);
        return -1;
    }

    rt = pStmt->execute();

    if (-1 == rt) {
        DEBUG_LOG("execute prepared sql failed.");
        return -1;
    }

    return 0;
}

ReturnStatus
LogAccessEngine::queryUserLogSeqNrFromArchivedLog(
    uint64_t bucketId,
    uint64_t userId,
    int logIdx,
    uint64_t *pSeqNr)
{
    int rt = 0;

    Connection *pConn = createConnectionToArchivedLog(bucketId, logIdx);

    if (NULL == pConn) {
        DEBUG_LOG("Cannot get database connection, bucket id %" PRIu64,
                  bucketId);

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    ConnectionGuard connGuard(&pConn);

    Statement *pStmt = pConn->createStatement();

    StatementGuard stmtGuard(&pStmt);

    std::string sql = "select seq_ from " USER_TABLE_NAME
                      " where uid_ = ?";

    rt = pStmt->prepare(sql);

    if (-1 == rt) {
        DEBUG_LOG("prepare statement failed, %s", sql.c_str());

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = pStmt->bindInt64(1, userId);

    if (-1 == rt) {
        DEBUG_LOG("bind parameter failed, index 1");
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = pStmt->execute();

    if (-1 == rt) {
        DEBUG_LOG("execute prepared sql failed.");

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    if (SQLITE_ROW == rt) {
        *pSeqNr = pStmt->columnInt64(0);

    } else {
        *pSeqNr = 0;
    }

    return ReturnStatus(MU_SUCCESS);
}

int
LogAccessEngine::flush(uint64_t bucketId)
{
    RWLockGuard lockGuard(
        getBucketLock(bucketId),
        RWLockGuard::WRITE
    );

    return flushLocked(bucketId);
}

int
LogAccessEngine::flushLocked(uint64_t bucketId)
{
    int rt = 0;

    Connection *pConn = getConnection(bucketId);

    if (NULL == pConn) {
        ERROR_LOG("flush log failed, can not connect to database, "
                  "bucket Id %" PRIu64, bucketId);
        return -1;
    }

    rt = pConn->commit();

    if (-1 == rt) {
        ERROR_LOG("flush log failed, commit transaction error, "
                  "bucket id %" PRIu64, bucketId);

        //pConn->rollback();

        return -1;
    }

    rt = pConn->begin();

    if (-1 == rt) {
        ERROR_LOG("begin transaction on log connection failed, bucket %"
                  PRIu64, bucketId);
        return -1;
    }

    return 0;
}


