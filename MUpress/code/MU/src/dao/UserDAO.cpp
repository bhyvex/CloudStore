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
#include "storage/ChannelManager.h"
#include "storage/Channel.h"
#include "storage/FSNameSpace.h"



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
	std::string pathname = 
			/* pChannel->m_Root +
			PATH_SEPARATOR_STRING +
			*/
			BUCKET_NAME_PREFIX +
			util::conv::conv<std::string, uint64_t>(bucketId) +
			PATH_SEPARATOR_STRING +
			USER_NAME_PREFIX +
			util::conv::conv<std::string, uint64_t>(userId);

	return pathname;
}


ReturnStatus
UserDAO::createUser(uint64_t bucketId,
                    uint64_t userId, uint64_t quota)
{
    int rt = 0;
    int error = 0;

    std::string userRoot = absUserRootPath(bucketId, userId);

	Channel* pDataChannel = ChannelManager::getInstance()->Mapping(bucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;
	Channel* pInfoChannel = ChannelManager::getInstance()->findChannel(MUConfiguration::getInstance()->m_MainChannelID);
	NameSpace *InfoNS = pInfoChannel->m_DataNS;

	rt = DataNS->MkDir(userRoot.c_str(), S_IRWXU);
    if (-1 == rt) {
        error = errno;
        ERROR_LOG("path %s, mkdir() error, %s.",
                  userRoot.c_str(), strerror(errno));

        if (EEXIST == error) {
            return ReturnStatus(MU_FAILED, USER_EXIST);
        }

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = InfoNS->MkDir(userRoot.c_str(), S_IRWXU);
    if (-1 == rt) {
        error = errno;
        ERROR_LOG("path %s, mkdir() error, %s.",
                  userRoot.c_str(), strerror(errno));

        if (EEXIST == error) {
            return ReturnStatus(MU_FAILED, USER_EXIST);
        }

        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    std::string infoFile = userRoot + PATH_SEPARATOR_STRING
                           + USER_INFO_FILE_NAME;

	Args fd_;
    fd_ = InfoNS->Open(infoFile.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

    if (false == fd_.valid) {
        ERROR_LOG("path %s, open() error, %s.",
                  infoFile.c_str(), strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    //int fd = fd_.arg2;

    struct UserInfo info;

    info.m_TotalQuota = quota;
    info.m_UsedQuota = 0;

    ReturnStatus rs;

    rs = writeUserInfo(&fd_, info);

    if (!rs.success()) {
        ERROR_LOG("bucket id %llu, user id %llu, "
                  "writeUserInfo() error", bucketId, userId);
    }

    InfoNS->Close(&fd_);

    return rs;
}

ReturnStatus
UserDAO::deleteUser(uint64_t bucketId, uint64_t userId)
{
    ReturnStatus rs;

    std::string userRoot = absUserRootPath(bucketId, userId);

    rs = rmUserRecursive(bucketId, userId);
    if (!rs.success()) {
        ERROR_LOG("path %s, rmdirRecursive() error", userRoot.c_str());
    }

    return rs;
}

ReturnStatus
UserDAO::readUserInfo(uint64_t bucketId,
                      uint64_t userId, UserInfo *pInfo)
{
    assert(pInfo);

    Args fd;

    std::string userRoot = absUserRootPath(bucketId, userId);
    std::string infoFile = userRoot + PATH_SEPARATOR_STRING
                           + USER_INFO_FILE_NAME;

	Channel* pChannel = ChannelManager::getInstance()->findChannel(MUConfiguration::getInstance()->m_MainChannelID);
	NameSpace *FSNS = pChannel->m_DataNS;
	
    fd = FSNS->Open(infoFile.c_str(), O_RDONLY);

    if (false == fd.valid) {
        ERROR_LOG("path %s, open() error, %s,",
                  infoFile.c_str(), strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    //int fd = rt;

    ReturnStatus rs;

    rs = readUserInfo(&fd, pInfo);

    FSNS->Close(&fd);

    return rs;
}

ReturnStatus
UserDAO::writeUserInfo(uint64_t bucketId,
                       uint64_t userId, const UserInfo &info)
{
    Args fd;

    std::string userRoot = absUserRootPath(bucketId, userId);
    std::string infoFile = userRoot + PATH_SEPARATOR_STRING
                           + USER_INFO_FILE_NAME;

    Channel* pChannel = ChannelManager::getInstance()->findChannel(MUConfiguration::getInstance()->m_MainChannelID);
	NameSpace *FSNS = pChannel->m_DataNS;

    fd = FSNS->Open(infoFile.c_str(), O_WRONLY);

    if (false == fd.valid) {
        ERROR_LOG("path %s, open() error, %s,",
                  infoFile.c_str(), strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    //int fd = rt;

    ReturnStatus rs;

    rs = writeUserInfo(&fd, info);

    FSNS->Close(&fd);

    return rs;

}

ReturnStatus
UserDAO::readUserInfo(Args* fd, UserInfo *pInfo)
{
    assert(pInfo);

    Channel* pInfoChannel = ChannelManager::getInstance()->findChannel(MUConfiguration::getInstance()->m_MainChannelID);
	NameSpace *InfoNS = pInfoChannel->m_DataNS;

    int rt = 0;


    rt = InfoNS->Lseek(fd, 0, SEEK_SET);

    if (-1 == rt) {
        ERROR_LOG("lseek error(), %s.", strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = InfoNS->readn(fd, pInfo, sizeof(*pInfo));

    if (sizeof(*pInfo) != rt) {
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);
}

ReturnStatus
UserDAO::writeUserInfo(Args* fd, const UserInfo &info)
{
	Channel* pInfoChannel = ChannelManager::getInstance()->findChannel(MUConfiguration::getInstance()->m_MainChannelID);
	NameSpace *InfoNS = pInfoChannel->m_DataNS;
	
    int rt = 0;

    rt = InfoNS->Lseek(fd, 0, SEEK_SET);

    if (-1 == rt) {
        ERROR_LOG("lseek error(), %s.", strerror(errno));
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    rt = InfoNS->writen(fd, &info, sizeof(info));

    if (sizeof(info) != rt) {
        ERROR_LOG("writen() failed, fd %d", fd);
        return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
    }

    return ReturnStatus(MU_SUCCESS);
}


ReturnStatus 
UserDAO::rmUserRecursive(uint64_t bucketId, uint64_t userId)
{
	Channel* pDataChannel = ChannelManager::getInstance()->Mapping(bucketId);
	NameSpace *DataNS = pDataChannel->m_DataNS;
	Channel* pInfoChannel = ChannelManager::getInstance()->findChannel(MUConfiguration::getInstance()->m_MainChannelID);
	NameSpace *InfoNS = pInfoChannel->m_DataNS;

	std::string userRoot = absUserRootPath(bucketId, userId);

	int ret;

	ReturnStatus rs;
	ret = DataNS->RmdirRecursive(userRoot.c_str());
	ret += InfoNS->RmdirRecursive(userRoot.c_str());

	if(ret != 0){
		return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
	}
	return ReturnStatus(MU_SUCCESS);
}


