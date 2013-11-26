/*
 * @file BucketDAO.cpp
 * @brief Access bucket-related data.
 *
 * @version 1.0
 * @date Wed Jul 11 16:01:01 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#include "BucketDAO.h"

#include "frame/MUDAO.h"
#include "frame/ReturnStatus.h"
#include "frame/MUConfiguration.h"
#include "frame/MUMacros.h"
#include "zip/Zip.h"

#include "log/log.h"
#include "util/util.h"

#include <dirent.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include <string>

BucketDAO::~BucketDAO()
{

}

BucketDAO::BucketDAO()
{

}

ReturnStatus
BucketDAO::deleteBucket(uint64_t bucketId)
{
    std::string bucketRoot = bucketRootPath(bucketId);

    return rmdirRecursive(bucketRoot);
}

ReturnStatus
BucketDAO::rmdirRecursive(const std::string &path)
{
    int rt = 0;
    ReturnStatus rs;
    int error = 0;
    std::string entryName;
    std::string npath;

    DIR *pDir = ::opendir(path.c_str());

    if (NULL == pDir) {
        error = errno;

        DEBUG_LOG("path %s, opendir() error, %s.",
                  path.c_str(), strerror(errno));

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    // delete its children

    struct dirent *pEnt = NULL;
    struct stat st;

    while (NULL != (pEnt = ::readdir(pDir))) {
        entryName = pEnt->d_name;

        // omit "." and ".."
        if (entryName == DENTRY_CURRENT_DIR
            || entryName == DENTRY_PARENT_DIR) {

            continue;
        }

        npath = path + PATH_SEPARATOR_STRING + entryName;

        if (DT_DIR == pEnt->d_type) {
            // directory, call myself to delete it
            rs = rmdirRecursive(npath);

            if (!rs.success()) {
                return rs;
            }

        } else {
            // delete it directly
            rt = ::unlink(npath.c_str());

            if (-1 == rt) {
                DEBUG_LOG("path %s, unlink() error, %s.",
                          npath.c_str(), strerror(errno));
                return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
            }
        }

        //rt = ::stat(npath.c_str(), &st);
        //if (-1 == rt) {
        //DEBUG_LOG("stat() error, path %s, %s",
        //npath.c_str(), strerror(errno));
        //return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        //}

        //if (S_ISDIR(st.st_mode)) {
        //// directory, call myself to delete it
        //return rmdirRecursive(npath);

        //} else {
        //// delete it directly
        //rt = ::unlink(npath.c_str());

        //if (-1 == rt) {
        //DEBUG_LOG("path %s, unlink() error, %s.",
        //npath.c_str(), strerror(errno));
        //return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        //}
        //}
    }

    ::closedir(pDir);

    // delete path

    rt = ::rmdir(path.c_str());

    if (-1 == rt) {
        DEBUG_LOG("path %s, rmdir() error, %s.",
                  path.c_str(), strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);
}

std::string
BucketDAO::bucketRootPath(uint64_t bucketId)
{
    return (
               MUConfiguration::getInstance()->m_FileSystemRoot +
               PATH_SEPARATOR_STRING +
               BUCKET_NAME_PREFIX +
               util::conv::conv<std::string, uint64_t>(bucketId)
           );
}

ReturnStatus
BucketDAO::createMigrationTmpFile(uint64_t bucketId, int *pFd)
{
    assert(pFd);

    std::string dataRoot = MUConfiguration::getInstance()->m_FileSystemRoot;

    std::string tmpFileName =
        dataRoot + PATH_SEPARATOR_STRING
        + BUCKET_MIGRATION_TMP_FILE_NAME_PREFIX
        + util::conv::conv<std::string, uint64_t>(bucketId)
        //+ ".tar.gz";
        + ".zip";

    int rt = 0;

    rt = ::open(tmpFileName.c_str(), O_WRONLY | O_TRUNC | O_CREAT,
                S_IRUSR | S_IWUSR);

    if (-1 == rt) {
        DEBUG_LOG("open() error, %s.", strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    *pFd = rt;

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
BucketDAO::openMigrationTmpFile(uint64_t bucketId, int *pFd)
{
    assert(pFd);

    std::string dataRoot = MUConfiguration::getInstance()->m_FileSystemRoot;

    std::string tmpFileName =
        dataRoot + PATH_SEPARATOR_STRING
        + BUCKET_MIGRATION_TMP_FILE_NAME_PREFIX
        + util::conv::conv<std::string, uint64_t>(bucketId)
        //+ ".tar.gz";
        + ".zip";

    int rt = 0;

    rt = ::open(tmpFileName.c_str(), O_RDONLY);

    if (-1 == rt) {
        DEBUG_LOG("open() error, %s.", strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    *pFd = rt;

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
BucketDAO::deleteMigrationTmpFile(uint64_t bucketId)
{
    std::string dataRoot = MUConfiguration::getInstance()->m_FileSystemRoot;

    std::string tmpFileName =
        dataRoot + PATH_SEPARATOR_STRING
        + BUCKET_MIGRATION_TMP_FILE_NAME_PREFIX
        + util::conv::conv<std::string, uint64_t>(bucketId)
        //+ ".tar.gz";
        + ".zip";

    int rt = 0;

    rt = ::unlink(tmpFileName.c_str());

    if (-1 == rt) {
        DEBUG_LOG("unlink() error, %s.", strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
BucketDAO::writeMigrationTmpFile(int fd, const std::string &data)
{
    int rt = 0;

    rt = util::io::writen(fd, data.c_str(), data.length());

    if (data.length() != rt) {
        DEBUG_LOG("writen() error, %s.", strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
BucketDAO::readMigrationTmpFile(int fd, std::string *pData)
{
    int rt = 0;

    char *pBuf = new char[MIGRATION_DATA_PIECE_SIZE];

    rt = util::io::readn(fd, pBuf, MIGRATION_DATA_PIECE_SIZE);

    if (-1 == rt) {
        DEBUG_LOG("readn() error, %s.", strerror(errno));

        delete [] pBuf;
        pBuf = NULL;

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    *pData = std::string(pBuf, rt);

    delete [] pBuf;
    pBuf = NULL;

    if (MIGRATION_DATA_PIECE_SIZE != rt) {
        // read EOF

        return ReturnStatus(MU_SUCCESS, MU_NO_MORE_DATA);

    } else {
        return ReturnStatus(MU_SUCCESS, MU_MORE_DATA);
    }
}

ReturnStatus
BucketDAO::extractMigrationTmpFile(uint64_t bucketId)
{
    std::string dataRoot = MUConfiguration::getInstance()->m_FileSystemRoot;

    std::string bucketRoot = bucketRootPath(bucketId);

    std::string tmpFileName =
        dataRoot + PATH_SEPARATOR_STRING
        + BUCKET_MIGRATION_TMP_FILE_NAME_PREFIX
        + util::conv::conv<std::string, uint64_t>(bucketId)
        //+ ".tar.gz";
        + ".zip";

    int rt = 0;

    rmdirRecursive(bucketRoot);

    //std::string command =
    //"tar xf " + tmpFileName + " -C " + dataRoot;

    //rt = ::system(command.c_str());

    //if (-1 == rt) {
    //DEBUG_LOG("system() error, %s.", strerror(errno));
    //return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    //}

    Zip *pZip = new Zip(tmpFileName);

    rt = pZip->uncompress(dataRoot);

    delete pZip;
    pZip = NULL;

    if (-1 == rt) {
        ERROR_LOG("try to unzip bucket %" PRIu64 " failed");
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
BucketDAO::tarMigrationTmpFile(uint64_t bucketId)
{
    std::string dataRoot = MUConfiguration::getInstance()->m_FileSystemRoot;

    //std::string bucketRootName =
    //BUCKET_NAME_PREFIX +
    //util::conv::conv<std::string, uint64_t>(bucketId);

    std::string tmpFileName =
        dataRoot + PATH_SEPARATOR_STRING
        + BUCKET_MIGRATION_TMP_FILE_NAME_PREFIX
        + util::conv::conv<std::string, uint64_t>(bucketId)
        //+ ".tar.gz";
        + ".zip";

    int rt = 0;

    //std::string command =
    //"tar czf " + tmpFileName + " -C " + dataRoot + " " + bucketRootName;

    //rt = ::system(command.c_str());

    //if (-1 == rt) {
    //DEBUG_LOG("system() error, %s.", strerror(errno));
    //return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    //}

    std::string bucketRoot = bucketRootPath(bucketId);

    Zip *pZip = new Zip(tmpFileName);

    rt = pZip->compress(bucketRoot);

    delete pZip;
    pZip = NULL;

    if (-1 == rt) {
        ERROR_LOG("try to zip bucket %" PRIu64 " failed", bucketId);
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
BucketDAO::getAllUsersInBucket(uint64_t bucketId,
                               std::list<uint64_t> *pUserList)
{
    assert(pUserList);

    int rt = 0;

    std::string bucketRoot = bucketRootPath(bucketId);

    std::string entryName;

    DIR *pDir = ::opendir(bucketRoot.c_str());

    if (NULL == pDir) {
        DEBUG_LOG("path %s, opendir() error, %s.",
                  bucketRoot.c_str(), strerror(errno));

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    // delete its children

    struct dirent *pEnt = NULL;
    uint64_t userId = 0;

    while (NULL != (pEnt = ::readdir(pDir))) {
        entryName = pEnt->d_name;

        if (DT_DIR == pEnt->d_type
            && 0 == entryName.find(USER_NAME_PREFIX)) {
            userId =
                util::conv::conv<uint64_t, std::string>(
                    entryName.substr(std::string(USER_NAME_PREFIX).length()));
            pUserList->push_back(userId);
        }
    }

    ::closedir(pDir);

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
BucketDAO::createBucket(uint64_t bucketId)
{
    int rt = 0;

    std::string bucketRoot = bucketRootPath(bucketId);

    rt = ::mkdir(bucketRoot.c_str(), S_IRWXU);

    if (-1 == rt) {
        DEBUG_LOG("mkdir() error, %s.", strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
BucketDAO::createBucketIfNotExist(uint64_t bucketId)
{
    std::string bucketRoot = bucketRootPath(bucketId);

    int rt = 0;
    struct stat st;

    rt = ::stat(bucketRoot.c_str(), &st);

    if (-1 == rt) {
        return createBucket(bucketId);
    }

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
BucketDAO::moveUser(
    uint64_t userId, uint64_t srcBucket, uint64_t destBucket)
{
    std::string srcPath = userRootPath(srcBucket, userId);
    std::string destPath = userRootPath(destBucket, userId);

    int rt = 0;

    rt = ::rename(srcPath.c_str(), destPath.c_str());

    if (-1 == rt) {
        DEBUG_LOG("reanem() error, %s.", strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
BucketDAO::linkLogFile(uint64_t srcBucket, uint64_t destBucket)
{
    int rt = 0;

    std::string srcLog = logPath(srcBucket);
    std::string destLog = logPath(destBucket);

    rt = ::link(srcLog.c_str(), destLog.c_str());

    if (-1 == rt) {
        DEBUG_LOG("link() error, %s.", strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);
}

std::string
BucketDAO::userRootPath(uint64_t bucketId, uint64_t userId)
{
    return (
               bucketRootPath(bucketId)
               + PATH_SEPARATOR_STRING
               + USER_NAME_PREFIX
               + util::conv::conv<std::string, uint64_t>(userId)
           );
}

std::string
BucketDAO::logPath(uint64_t bucketId)
{
    return (
               MUConfiguration::getInstance()->m_FileSystemRoot +
               PATH_SEPARATOR_STRING +
               BUCKET_NAME_PREFIX +
               util::conv::conv<std::string, uint64_t>(bucketId) +
               PATH_SEPARATOR_STRING +
               BUCKET_LOG_FILE_NAME
           );
}


