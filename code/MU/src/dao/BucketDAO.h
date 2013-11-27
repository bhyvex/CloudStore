/*
 * @file BucketDAO.h
 * @brief Access bucket-related data.
 *
 * @version 1.0
 * @date Wed Jul 11 15:56:36 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef dao_BucketDAO_H_
#define dao_BucketDAO_H_

#include "frame/MUDAO.h"
#include "frame/ReturnStatus.h"

#include <inttypes.h>

#include <string>
#include <list>

class BucketDAO : public MUDAO
{
public:
    virtual ~BucketDAO();

    BucketDAO();

    ReturnStatus deleteBucket(uint64_t bucketId);

    ReturnStatus openMigrationTmpFile(uint64_t bucketId, int *pFd);

    ReturnStatus createMigrationTmpFile(uint64_t bucketId, int *pFd);

    ReturnStatus deleteMigrationTmpFile(uint64_t bucketId);

    ReturnStatus writeMigrationTmpFile(int fd, const std::string &data);

    ReturnStatus readMigrationTmpFile(int fd, std::string *pData);

    ReturnStatus extractMigrationTmpFile(uint64_t bucketId);

    ReturnStatus tarMigrationTmpFile(uint64_t bucketId);

    ReturnStatus getAllUsersInBucket(uint64_t bucketId,
                                     std::list<uint64_t> *pUserList);

    ReturnStatus createBucket(uint64_t bucketId);

    ReturnStatus createBucketIfNotExist(uint64_t bucketId);

    ReturnStatus moveUser(
        uint64_t userId, uint64_t srcBucket, uint64_t destBucket);

    ReturnStatus linkLogFile(uint64_t srcBucket, uint64_t destBucket);

private:
    /**
     * @brief Delete a specified directory, even it's not empty.
     *
     * @param path
     *
     * @return
     */
    ReturnStatus rmdirRecursive(const std::string &path);

    std::string bucketRootPath(uint64_t bucketId);

    std::string userRootPath(uint64_t bucketId, uint64_t userId);

    std::string logPath(uint64_t bucketId);
};

#endif  // dao_BucketDAO_H_

