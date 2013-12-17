/*
 * @file LogFileDAO.cpp
 * @brief Log file operations.
 *
 * @version 1.0
 * @date Tue Jul 10 14:22:57 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "LogFileDAO.h"

#include "frame/ReturnStatus.h"
#include "frame/MUConfiguration.h"
#include "frame/MUMacros.h"
#include "storage/ChannelManager.h"
#include "storage/Channel.h"
#include "storage/NameSpace.h"
#include "storage/FSNameSpace.h"

#include "protocol/MUMacros.h"


#include "log/log.h"
#include "util/util.h"

#include <list>
#include <map>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

LogFileDAO::~LogFileDAO()
{

}

LogFileDAO::LogFileDAO()
{

}


ReturnStatus
LogFileDAO::checkLogFileSize(
    const std::list<uint64_t> &bucketList,
    std::map<uint64_t, uint64_t> *pLogMap)
{
    int rt = 0;
    struct stat st;

    for (std::list<uint64_t>::const_iterator it = bucketList.begin();
         it != bucketList.end(); ++it) {
        rt = ::stat(logPath(*it).c_str(), &st);

        if (-1 == rt) {
            ERROR_LOG("Check log file size failed, path %s.",
                      logPath(*it).c_str());
            return ReturnStatus(MU_FAILED, MU_UNKNOWN_ERROR);
        }

        pLogMap->insert(std::pair<uint64_t, uint64_t>(*it, st.st_size));
    }

    return ReturnStatus(MU_SUCCESS);
}


std::string
LogFileDAO::logPath(uint64_t bucketId)
{
	Channel* pInfoChannel = ChannelManager::getInstance()->findChannel(MUConfiguration::getInstance()->m_MainChannelID);
	NameSpace *InfoNS = pInfoChannel->m_DataNS;

    return (
               InfoNS->m_Root +
               PATH_SEPARATOR_STRING +
               BUCKET_NAME_PREFIX +
               util::conv::conv<std::string, uint64_t>(bucketId) +
               PATH_SEPARATOR_STRING +
               BUCKET_LOG_FILE_NAME
           );
}




