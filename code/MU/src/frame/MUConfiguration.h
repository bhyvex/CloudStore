/*
 * @file MUConfiguration.h
 * @brief Global configuration.
 *
 * @version 1.0
 * @date Mon Jul  2 17:25:42 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef _MUCONFIGURATION_H_
#define _MUCONFIGURATION_H_

#include <iostream>
#include <string>
#include <map>

#include <sys/types.h>
#include <inttypes.h>

#include "sys/sys.h"
#include "log/log.h"

class MUConfiguration : public Singleton<MUConfiguration>
{
    friend class Singleton<MUConfiguration>;

public:
    // queue of listen
    int m_ListenQueue;
    // limit of open files
    int m_LimitNOFile;
    // threads in thread pool
    int m_ThreadPoolWorkers;

    // CS info
    std::string m_CSIP;
    uint16_t m_CSPort;

    // RS info
    std::string m_RSIP;
    uint16_t m_RSPort;

    // heartbeat with CS
    int m_CSHeartbeatKeepAlive;
    int m_CSHeartbeatTimeOut;

    // heartbeat between MUs
    int m_MUHeartbeatKeepAlive;
    int m_MUHeartbeatTimeOut;

    // client-processing module
    std::string m_ClientPMListenIP;
    uint16_t m_ClientPMListenPort;

    // CS-processing module
    std::string m_CSPMListenIP;
    uint16_t m_CSPMListenPort;

    // Synchronization module
    std::string m_SyncListenIP;
    uint16_t m_SyncListenPort;

    // Migration module
    std::string m_MigrationIP;
    uint16_t m_MigrationPort;


	//storage
	int m_ChannelNum;
	int m_MainChannelID;
	std::map<int, std::string> m_ChannelVec;

    // file system root
    std::string m_FileSystemRoot;
    // user serializable lock
    bool m_UserSerializable;

    // oplogs
    std::string m_LogRotateStrategy;
    uint64_t m_LogRotateTime;
    uint64_t m_LogRotateSize;
    int m_RotateLogFiles;

    // log subsystem
    LogLevel m_LogLevel;

public:
    virtual ~MUConfiguration();

    int configWithXML(const std::string &configFileName);

private:
    // can't new
    MUConfiguration();
    // can't copy
    MUConfiguration(const MUConfiguration &conf);
    MUConfiguration &operator=(const MUConfiguration &conf);
};

#endif  // _MUCONFIGURATION_H_

