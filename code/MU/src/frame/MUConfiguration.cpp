/*
 * @file MUConfiguration.cpp
 * @brief Global configurations.
 *
 * @version 1.0
 * @date Mon Jul  2 17:28:37 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "MUConfiguration.h"

#include <stdio.h>

#include <string>

#include <sys/types.h>

#include "xml/Xml.h"
#include "util/util.h"
#include "log/log.h"

#include "MUMacros.h"

MUConfiguration::~MUConfiguration()
{

}

MUConfiguration::MUConfiguration()
{

}

int
MUConfiguration::configWithXML(const std::string &confFileName)
{
    int rt = 0;

    std::string val;

    Xml x;

    xmlNode *pRoot = x.loadFile(confFileName);

    if (NULL == pRoot) {
        return -1;
    }

    rt = x.getNodeValueByXPath("/MU/System/ListenQueue", &val);

    if (-1 == rt) {
        return -1;
    }

    m_ListenQueue = util::conv::conv<int, std::string>(val);

    rt = x.getNodeValueByXPath("/MU/System/LimitNOFile", &val);

    if (-1 == rt) {
        return -1;
    }

    std::string logLevel;
    rt = x.getNodeValueByXPath("/MU/System/LogLevel", &logLevel);

    if (-1 == rt) {
        return -1;
    }

    if (logLevel == "TRACE") {
        m_LogLevel = LOG_TRACE;
    } else if (logLevel == "DEBUG") {
        m_LogLevel = LOG_DEBUG;
    } else if (logLevel == "INFO") {
        m_LogLevel = LOG_INFO;
    } else if (logLevel == "WARN") {
        m_LogLevel = LOG_WARN;
    } else if (logLevel == "ERROR") {
        m_LogLevel = LOG_ERROR;
    } else if (logLevel == "FATAL") {
        m_LogLevel = LOG_FATAL;
    } else {
        fprintf(stderr, "Invalid log level %s.", logLevel.c_str());
        return -1;
    }

    m_LimitNOFile = util::conv::conv<int, std::string>(val);

    rt = x.getNodeValueByXPath("/MU/System/ThreadPool/Worker", &val);

    if (-1 == rt) {
        return -1;
    }

    m_ThreadPoolWorkers = util::conv::conv<int, std::string>(val);

    rt = x.getNodeValueByXPath("/MU/CS/IP", &val);

    if (-1 == rt) {
        return -1;
    }

    m_CSIP = val;

    rt = x.getNodeValueByXPath("/MU/CS/Port", &val);

    if (-1 == rt) {
        return -1;
    }

    m_CSPort = util::conv::conv<uint16_t, std::string>(val);

    rt = x.getNodeValueByXPath("/MU/RS/IP", &val);

    if (-1 == rt) {
        return -1;
    }

    m_RSIP = val;

    rt = x.getNodeValueByXPath("/MU/RS/Port", &val);

    if (-1 == rt) {
        return -1;
    }

    m_RSPort = util::conv::conv<uint16_t, std::string>(val);

    rt = x.getNodeValueByXPath("/MU/Heartbeat/CS/KeepAlive", &val);

    if (-1 == rt) {
        return -1;
    }

    m_CSHeartbeatKeepAlive = util::conv::conv<int, std::string>(val);

    rt = x.getNodeValueByXPath("/MU/Heartbeat/CS/TimeOut", &val);

    if (-1 == rt) {
        return -1;
    }

    m_CSHeartbeatTimeOut = util::conv::conv<int, std::string>(val);

    rt = x.getNodeValueByXPath("/MU/Heartbeat/MU/KeepAlive", &val);

    if (-1 == rt) {
        return -1;
    }

    m_MUHeartbeatKeepAlive = util::conv::conv<int, std::string>(val);

    rt = x.getNodeValueByXPath("/MU/Heartbeat/MU/TimeOut", &val);

    if (-1 == rt) {
        return -1;
    }

    m_MUHeartbeatTimeOut = util::conv::conv<int, std::string>(val);

    rt = x.getNodeValueByXPath("/MU/ClientPM/ListenIP", &val);

    if (-1 == rt) {
        return -1;
    }

    m_ClientPMListenIP = val;

    rt = x.getNodeValueByXPath("/MU/ClientPM/ListenPort", &val);

    if (-1 == rt) {
        return -1;
    }

    m_ClientPMListenPort = util::conv::conv<uint16_t, std::string>(val);

    rt = x.getNodeValueByXPath("/MU/CSPM/ListenIP", &val);

    if (-1 == rt) {
        return -1;
    }

    m_CSPMListenIP = val;

    rt = x.getNodeValueByXPath("/MU/CSPM/ListenPort", &val);

    if (-1 == rt) {
        return -1;
    }

    m_CSPMListenPort = util::conv::conv<uint16_t, std::string>(val);

    rt = x.getNodeValueByXPath("/MU/MigrationPM/ListenIP", &val);

    if (-1 == rt) {
        return -1;
    }

    m_MigrationIP = val;

    rt = x.getNodeValueByXPath("/MU/MigrationPM/ListenPort", &val);

    if (-1 == rt) {
        return -1;
    }

    m_MigrationPort = util::conv::conv<uint16_t, std::string>(val);

    rt = x.getNodeValueByXPath("/MU/SyncPM/ListenIP", &val);

    if (-1 == rt) {
        return -1;
    }

    m_SyncListenIP = val;

    rt = x.getNodeValueByXPath("/MU/SyncPM/ListenPort", &val);

    if (-1 == rt) {
        return -1;
    }

    m_SyncListenPort = util::conv::conv<uint16_t, std::string>(val);


	rt = x.getNodeValueByXPath("/MU/Storage/ChannelNum", &val);
	if (-1 == rt) {
        return -1;
    }
	m_ChannelNum = util::conv::conv<int, std::string>(val);
	std::cout <<"/MU/Storage/ChannelNum = "<< m_ChannelNum <<std::endl;

	rt = x.getNodeValueByXPath("/MU/Storage/MainChannelID", &val);
	if (-1 == rt) {
        return -1;
    }
	m_MainChannelID = util::conv::conv<int, std::string>(val);
	std::cout <<"/MU/Storage/MainChannelID = "<< m_MainChannelID <<std::endl;

	for(int i = 0; i < m_ChannelNum; i ++){
	
		std::string key = "/MU/Storage/Channel";
		key += util::conv::conv<std::string, int>(i);
		key += "_Root";
		std::cout <<"  key = "<< key <<std::endl;
		
		rt = x.getNodeValueByXPath(key.c_str(), &val);
		if (-1 == rt) {
        	return -1;
    	}

    	m_ChannelVec.insert(std::pair<int, std::string>(i, val));
    	std::cout <<"/MU/Storage/m_ChannelVec["<<i<<"] = "<< m_ChannelVec[i] <<std::endl;
    	
	}

    rt = x.getNodeValueByXPath("/MU/User/UserSerializable", &val);

    if (-1 == rt) {
        return -1;
    }

    if (val == "true") {
        m_UserSerializable = true;

    } else if (val == "false") {
        m_UserSerializable = false;

    } else {
        fprintf(stderr, "Unexpected user searializable value %s.", val.c_str());
        return -1;
    }
    std::cout <<"/MU/User/UserSerializable = "<< m_UserSerializable <<std::endl;

    rt = x.getNodeValueByXPath("/MU/Log/RotateStrategy", &val);

    if (-1 == rt) {
        return -1;
    }

    if (val == LOG_ROTATION_STRATEGY_SIZE ||
        val == LOG_ROTATION_STRATEGY_TIME) {
        m_LogRotateStrategy = val;
        std::cout <<"/MU/Log/RotateStrategy = "<< m_LogRotateStrategy <<std::endl;

    } else {
        fprintf(stderr, "Unexpected log rotation strategy %s.", val.c_str());
        return -1;
    }

    rt = x.getNodeValueByXPath("/MU/Log/RotateTime", &val);

    if (-1 == rt) {
        return -1;
    }

    m_LogRotateTime = util::conv::conv<uint64_t, std::string>(val);
    std::cout <<"/MU/Log/RotateTime = "<< m_LogRotateTime <<std::endl;

    rt = x.getNodeValueByXPath("/MU/Log/RotateSize", &val);

    if (-1 == rt) {
        return -1;
    }

    m_LogRotateSize = util::conv::conv<uint64_t, std::string>(val);
    std::cout <<"/MU/Log/RotateSize = "<< m_LogRotateSize <<std::endl;

    rt = x.getNodeValueByXPath("/MU/Log/RotateLogFiles", &val);

    if (-1 == rt) {
        return -1;
    }

    m_RotateLogFiles = util::conv::conv<int, std::string>(val);
    std::cout <<"/MU/Log/RotateLogFiles = "<< m_RotateLogFiles <<std::endl;

    if (m_RotateLogFiles < LEAST_ARCHIVED_LOG_FILES) {
        fprintf(stderr, "Invalid number of archived log files %" PRIi32,
                m_RotateLogFiles);
        return -1;
    }

    return 0;
}

