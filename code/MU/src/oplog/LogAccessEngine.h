/*
 * @file LogAccessEngine.h
 * @brief You can do all log operations via this class.
 *
 * @version 1.0
 * @date Mon Jul  9 15:12:23 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef _LogAccessEngine_H_
#define _LogAccessEngine_H_

#include "LogEvent.h"
#include "Connection.h"

#include "sys/sys.h"
#include "frame/ReturnStatus.h"

#include <map>
#include <string>

class LogAccessEngine : public Singleton<LogAccessEngine>
{
    friend class Singleton<LogAccessEngine>;
public:
    virtual ~LogAccessEngine();

    /**
     * @brief Write a log record to database.
     *
     * @param event
     *
     * @return
     */
    int write(const LogEvent &event);

    /**
     * @brief Rotate log files.
     *
     * @param bucketId
     * @param totalBuckets total buckets in current system
     * @param event log the rotation operation
     *
     * @return
     */
    int rotate(
        uint64_t bucketId,
        uint64_t totalBuckets,
        const LogEvent &event
    );

    /**
     * @brief Close log file.
     *
     * @param bucketId
     *
     * @return
     */
    int close(uint64_t bucketId);

    /**
     * @brief flush log to local disk
     *
     * @param bucketId
     *
     * @return
     */
    int flush(uint64_t bucketId);


    /**
     * @brief create a new user in specified bucket
     *
     * @param bucketId
     * @param userId
     *
     * @return
     */
    int createUser(uint64_t bucketId, uint64_t userId);

    /**
     * @brief delete specified user
     *
     * @param bucketId
     * @param userId
     *
     * @return
     */
    int deleteUser(uint64_t bucketId, uint64_t userId);

    /**
     * @brief try to create a new log file if not exists
     *
     * @param bucketId
     *
     * @return
     */
    ReturnStatus createLogFile(uint64_t bucketId);

    /**
     * @brief Read user-specific logs
     *
     * @param bucketId
     * @param userId
     * @param pLogList [out]
     * @param pMore [out] more logs to read?
     *
     * @return
     */
    ReturnStatus readUserLog(
        uint64_t bucketId,
        uint64_t userId,
        uint64_t seq,
        std::list<LogEvent> *pLogList,
        bool *pMore
    );

    /**
     * @brief Get current max log sequence of specified bucket.
     *
     * @param bucketId
     * @param pSeqNr [out]
     *
     * @return
     */
    ReturnStatus queryCurrentBucketLogSeqNr(
        uint64_t bucketId, uint64_t *pSeqNr);

    /**
     * @brief Get current max log sequence of specified user.
     *
     * @param bucketId
     * @param userId
     * @param pSeqNr [out]
     *
     * @return
     */
    ReturnStatus queryCurrentUserLogSeqNr(
        uint64_t bucketId, uint64_t userId, uint64_t *pSeqNr);

protected:
    // cannot new
    LogAccessEngine();

protected:
    /**
     * @brief Has physical model in specified database?
     *
     * @param pConn
     *
     * @return
     */
    bool hasModel(Connection *pConn);

    /**
     * @brief Create physical model to store logs,
     *          including tables and indexes.
     *
     * @return
     */
    int createModel(Connection *pConn);

    /**
     * @brief Create log table.
     *
     * @return
     */
    int createTable(Connection *pConn);

    /**
     * @brief create indexes on log table to accerlarate query speed.
     *
     * @return
     */
    int createIndex(Connection *pConn);

    /**
     * @brief create triggers on log table to update user table
     *
     * @param pConn
     *
     * @return
     */
    int createTrigger(Connection *pConn);

    /**
     * @brief Get a database connection from connection map
     *          or create a new one.
     *
     * @param bucketId
     *
     * @return
     */
    Connection *getConnection(uint64_t bucketId);

    /**
     * @brief Create a new database connection
     *
     * @param bucketId
     * @param create create a new one if not exists?
     * @param put new connection in m_ConnMap?
     *
     * @return
     */
    Connection *createConnection(uint64_t bucketId, bool create, bool inMap);

    /**
     * @brief create connection to an archived log file,
     *          if logIdx is 0, connect to current log file
     *
     * @param bucketId
     * @param logIdx log file index
     *
     * @return
     */
    Connection *createConnectionToArchivedLog(uint64_t bucketId, int logIdx);

    /**
     * @brief Close an existed database connection.
     *
     * @param bucketId
     */
    void closeConnection(uint64_t bucketId);

    /**
     * @brief copy user table from bucket_log.1 to new bucket_log,
     *          we should also filter out some users which do not
     *          belong to bucketId any more
     *
     * @param bucketId
     * @param totalBuckets
     *
     * @return
     */
    int copyUserTable(uint64_t bucketId, uint64_t totalBuckets);

    /**
     * @brief query max seq nr from user table
     *
     * @param bucketId
     * @param
     *
     * @return
     */
    ReturnStatus queryCurrentBucketLogSeqNrFromUserTable(
        uint64_t bucketId,
        uint64_t *pSeqNr
    );


    /**
     * @brief read user log from current log file
     *
     * @param bucketId
     * @param userId
     * @param seq
     * @param pLogList
     * @param
     *
     * @return
     */
    ReturnStatus readUserLogFromCurrentLog(
        uint64_t bucketId,
        uint64_t userId,
        uint64_t seq,
        std::list<LogEvent> *pLogList,
        bool *pMore
    );

    /**
     * @brief read user log from archived log file
     *
     * @param bucketId
     * @param userId
     * @param seq
     * @param pLogList
     * @param
     *
     * @return
     */
    ReturnStatus readUserLogFromArchivedLog(
        uint64_t bucketId,
        uint64_t userId,
        uint64_t seq,
        int logIdx,
        std::list<LogEvent> *pLogList,
        bool *pMore
    );

    /**
     * @brief whether log 'seq' existed in archive log file 'logIdx'
     *
     * @param bucketId
     * @param userId
     * @param seq
     * @param logIdx
     * @param pExist [out]
     *
     * @return
     */
    ReturnStatus userLogExistInArchiveLog(
        uint64_t bucketId,
        uint64_t userId,
        uint64_t seq,
        int logIdx,
        bool *pExist
    );

    /**
     * @brief query max user log seq in archived log 'logIdx'
     *
     * @param bucketId
     * @param userId
     * @param logIdx
     * @param pSeqNr
     *
     * @return
     */
    ReturnStatus queryUserLogSeqNrFromArchivedLog(
        uint64_t bucketId,
        uint64_t userId,
        int logIdx,
        uint64_t *pSeqNr);

    /**
     * @brief write a log record when bucket lock is already acquired
     *
     * @param event
     *
     * @return
     */
    int writeLogLocked(const LogEvent &event);

    /**
     * @brief flush logs when bucket lock is already acquired
     *
     * @param bucketId
     *
     * @return 
     */
    int flushLocked(uint64_t bucketId);

    /**
     * @brief get rwlock of specified bucket
     *
     * @param bucketId
     *
     * @return
     */
    RWLock *getBucketLock(uint64_t bucketId);

    /**
     * @brief construct log file path
     *
     * @param bucketId
     *
     * @return
     */
    std::string logPath(uint64_t bucketId);

    /**
     * @brief construct archvied log file path, if logIdx is 0,
     *          then return logPath(bucketId)
     *
     * @param bucketId
     * @param logIdx
     *
     * @return
     */
    std::string logPath(uint64_t bucketId, int logIdx);

    /**
     * @brief construct root path of specified bucket
     *
     * @param bucketId
     *
     * @return
     */
    std::string bucketRootPath(uint64_t bucketId);

    /**
     * @brief path existed and is a regular file?
     *
     * @param path
     *
     * @return
     */
    bool isfile(std::string &path);

private:
    // connections for log thread and only for log thread
    std::map<uint64_t, Connection *> m_ConnMap;
    // lock for each bucket, when we do log-related operations,
    // we have to lock it
    std::map<uint64_t, RWLock *> m_LockMap;
    // to protect m_LockMap
    Mutex m_LockMapMutex;
};

#endif  // _LogAccessEngine_H_

