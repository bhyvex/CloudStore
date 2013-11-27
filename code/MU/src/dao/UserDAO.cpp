/*
 * @file UserDAO.cpp
 * @brief User-related data access object.
 *
 * @version 1.0
 * @date Thu Jul  5 16:14:11 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "UserDAO.h"

#include "frame/ReturnStatus.h"
#include "frame/MUConfiguration.h"
#include "frame/MUMacros.h"
#include "data/UserInfo.h"

#include "log/log.h"
#include "util/util.h"

#include <inttypes.h>
#include <assert.h>

#include <string>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

UserDAO::~UserDAO()
{

}

UserDAO::UserDAO()
{

}

std::string
UserDAO::absUserRootPath(uint64_t bucketId, uint64_t userId)
{
    return (MUConfiguration::getInstance()->m_FileSystemRoot +
            PATH_SEPARATOR_STRING +
            BUCKET_NAME_PREFIX +
            util::conv::conv<std::string, uint64_t>(bucketId) +
            PATH_SEPARATOR_STRING +
            USER_NAME_PREFIX +
            util::conv::conv<std::string, uint64_t>(userId));

}

ReturnStatus
UserDAO::createUser(uint64_t bucketId,
                    uint64_t userId, uint64_t quota)
{
    int rt = 0;
    int error = 0;

    std::string userRoot = absUserRootPath(bucketId, userId);

    rt = ::mkdir(userRoot.c_str(), S_IRWXU);

    if (-1 == rt) {
        error = errno;
        DEBUG_LOG("path %s, mkdir() error, %s.",
                  userRoot.c_str(), strerror(errno));

        if (EEXIST == error) {
            return ReturnStatus(MU_FAILED, USER_EXIST);
        }

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    std::string infoFile = userRoot + PATH_SEPARATOR_STRING
                           + USER_INFO_FILE_NAME;

    rt = ::open(infoFile.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

    if (-1 == rt) {
        DEBUG_LOG("path %s, open() error, %s.",
                  infoFile.c_str(), strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    int fd = rt;

    struct UserInfo info;

    info.m_TotalQuota = quota;
    info.m_UsedQuota = 0;

    ReturnStatus rs;

    rs = writeUserInfo(fd, info);

    if (!rs.success()) {
        DEBUG_LOG("bucket id %llu, user id %llu, "
                  "writeUserInfo() error", bucketId, userId);
    }

    ::close(fd);

    return rs;
}

ReturnStatus
UserDAO::deleteUser(uint64_t bucketId, uint64_t userId)
{
    ReturnStatus rs;

    std::string userRoot = absUserRootPath(bucketId, userId);

    rs = rmdirRecursive(userRoot);

    if (!rs.success()) {
        DEBUG_LOG("path %s, rmdirRecursive() error", userRoot.c_str());
    }

    return rs;
}

ReturnStatus
UserDAO::readUserInfo(uint64_t bucketId,
                      uint64_t userId, UserInfo *pInfo)
{
    assert(pInfo);

    int rt = 0;

    std::string userRoot = absUserRootPath(bucketId, userId);
    std::string infoFile = userRoot + PATH_SEPARATOR_STRING
                           + USER_INFO_FILE_NAME;

    rt = ::open(infoFile.c_str(), O_RDONLY);

    if (-1 == rt) {
        DEBUG_LOG("path %s, open() error, %s,",
                  infoFile.c_str(), strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    int fd = rt;

    ReturnStatus rs;

    rs = readUserInfo(fd, pInfo);

    ::close(fd);

    return rs;
}

ReturnStatus
UserDAO::writeUserInfo(uint64_t bucketId,
                       uint64_t userId, const UserInfo &info)
{
    int rt = 0;

    std::string userRoot = absUserRootPath(bucketId, userId);
    std::string infoFile = userRoot + PATH_SEPARATOR_STRING
                           + USER_INFO_FILE_NAME;

    rt = ::open(infoFile.c_str(), O_WRONLY);

    if (-1 == rt) {
        DEBUG_LOG("path %s, open() error, %s,",
                  infoFile.c_str(), strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    int fd = rt;

    ReturnStatus rs;

    rs = writeUserInfo(fd, info);

    ::close(fd);

    return rs;

}

ReturnStatus
UserDAO::readUserInfo(int fd, UserInfo *pInfo)
{
    assert(pInfo);

    int rt = 0;

    rt = ::lseek(fd, 0, SEEK_SET);

    if (-1 == rt) {
        DEBUG_LOG("lseek error(), %s.", strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = util::io::readn(fd, pInfo, sizeof(*pInfo));

    if (sizeof(*pInfo) != rt) {
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
UserDAO::writeUserInfo(int fd, const UserInfo &info)
{
    int rt = 0;

    rt = ::lseek(fd, 0, SEEK_SET);

    if (-1 == rt) {
        DEBUG_LOG("lseek error(), %s.", strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = util::io::writen(fd, &info, sizeof(info));

    if (sizeof(info) != rt) {
        DEBUG_LOG("writen() failed, fd %d", fd);
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);
}


ReturnStatus
UserDAO::rmdirRecursive(const std::string &path)
{
    int rt = 0;
    ReturnStatus rs;
    std::string entryName;
    std::string npath;

    DIR *pDir = ::opendir(path.c_str());

    if (NULL == pDir) {
        DEBUG_LOG("path %s, opendir() error, %s.",
                  path.c_str(), strerror(errno));

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    // delete its children

    struct dirent *pEnt = NULL;

    while (NULL != (pEnt = ::readdir(pDir))) {
        entryName = pEnt->d_name;

        // omit "." and ".."
        if (entryName == DENTRY_CURRENT_DIR
            || entryName == DENTRY_PARENT_DIR) {
            //|| entryName == USER_INFO_FILE_NAME) {
            continue;
        }

        npath = path + PATH_SEPARATOR_STRING + entryName;

        if (DT_DIR == pEnt->d_type) {
            // directory, call myself to delete it
            rs = rmdirRecursive(npath);

            if (!rs.success()) {
                DEBUG_LOG("path %s, rmdirRecursive() error", npath.c_str());
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


