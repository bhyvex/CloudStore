/*
 * @file RunControl.cpp
 * @brief Program start controller.
 *
 * @version 1.0
 * @date Thu Jun 28 21:41:21 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "RunControl.h"

#include "MUConfiguration.h"
#include "MURegister.h"
#include "MUTCPListenAgent.h"
#include "MUTaskManager.h"
#include "RecycleTask.h"
#include "MUEpoll.h"

#include "sys/sys.h"
#include "log/log.h"
#include "DevLog/DevLog.h"

#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>

#include "oplog/LogDispatcher.h"
#include "oplog/LogRotationTask.h"
#include "oplog/LogFlushTask.h"
#include "init/InitTask.h"
#include "CSPM/CSHeartbeatTask.h"
#include "CSPM/CSTask.h"
#include "MigrationPM/MigrationServerTask.h"
#include "state/CheckIdleUserTask.h"
#include "storage/ChannelManager.h"
#include "storage/Channel.h"


// required by communication base classes
DevLog *g_pDevLog = NULL;

RunControl::~RunControl()
{

}

RunControl::RunControl()
{

}

void
RunControl::run()
{
    Epoll *pEpoll = NULL;
    pEpoll = MURegister::getInstance()->getEpoll();

    assert(pEpoll);

    pEpoll->run();
}


void
RunControl::initialize()
{
    initLog();

    setResourceLimit();

    initDevLog();

    initEpoll();

    initThreadPool();

    initChannel();

    initFilesystemRoot();

    //initRecycleTask();

    initOpLog();

    initCSHeartbeat();

    initMigrationListen();

    initCSListen();

    initCheckIdleUserTask();

    startSystem();
}

void
RunControl::setResourceLimit()
{
    int rt = 0;

    rt = ::geteuid();

    if (-1 == rt) {
        DEBUG_LOG("geteuid() error, %s", strerror(errno));
        ERROR_LOG("Cannot get EUID of current process. Program will exit.");
        exit(1);
    }

    int euid = rt;

    if (0 != euid) {
        // EUID is not "root", return now.
        // setrlimit() need root to run it
        return ;
    }

    int nofile = 0;
    nofile = MUConfiguration::getInstance()->m_LimitNOFile;

    struct rlimit rl;
    rl.rlim_cur = nofile;
    rl.rlim_max = nofile;

    rt = setrlimit(RLIMIT_NOFILE, &rl);

    if (-1 == rt) {
        DEBUG_LOG("setrlimit() error, %s", strerror(errno));
        ERROR_LOG("Can't set RLIMIT_NOFILE. Program will exit.");
        exit(1);
    }

    INFO_LOG("Set RLIMIT_NOFILE to %d.", nofile);
}

void
RunControl::initDevLog()
{
    int rt = 0;

    // init DevLog
    g_pDevLog = new DevLog();

    rt = g_pDevLog->init();

    if (-1 == rt) {
        ERROR_LOG("Initialize DevLog failed. Program will exit.");
        exit(1);
    }
}

void 
RunControl::initLog()
{
    int rt = LoggerFactory::getInstance()->initialize(
            MUConfiguration::getInstance()->m_LogLevel);

    if (-1 == rt) {
        fprintf(stderr, "Initalize log subsystem failed. Program will exit.");
        exit(1);
    }
}

void
RunControl::initEpoll()
{
    int rt = 0;

    // init Epoll
    Epoll *pEpoll = new MUEpoll();
    pEpoll->initialize(MUConfiguration::getInstance()->m_LimitNOFile);

    if (-1 == rt) {
        ERROR_LOG("Can't initalize epoll context. Program will exit.");
        exit(1);
    }

    MURegister::getInstance()->setEpoll(pEpoll);
}

void
RunControl::initThreadPool()
{
    int rt = 0;

    Epoll *pEpoll = NULL;
    pEpoll = MURegister::getInstance()->getEpoll();

    // init thread pool
    ThreadPool *pThreadPool = new ThreadPool(
        MUConfiguration::getInstance()->m_ThreadPoolWorkers);
    pThreadPool->start();

    if (-1 == rt) {
        ERROR_LOG("Can't initialize thread pool. Program will exit.");
        exit(1);
    }

    MURegister::getInstance()->setThreadPool(pThreadPool);

    INFO_LOG(
        "Initialize thread pool with %d workers.",
        MUConfiguration::getInstance()->m_ThreadPoolWorkers
    );

    // init MU thread pool dispatcher
    MUTPDispatcher *pTPDispatcher = new MUTPDispatcher(pEpoll, pThreadPool);

    MURegister::getInstance()->setThreadPoolDispatcher(pTPDispatcher);
}

void
RunControl::initOpLog()
{
    int rt = 0;

    Epoll *pEpoll = NULL;
    pEpoll = MURegister::getInstance()->getEpoll();

    // init log dispatcher
    LogDispatcher *pLogDispatcher = new LogDispatcher(pEpoll);
    MURegister::getInstance()->setLogDispatcher(pLogDispatcher);

    // init log rotation task
    LogRotationTask *pRotationTask =
        MUTaskManager::getInstance()->createTask<LogRotationTask>(NULL);

    rt = pRotationTask->start();

    if (-1 == rt) {
        ERROR_LOG("Initialize operation log module failed. "
                  "Program will exit.");
        exit(1);
    }

    // init log flush task
    LogFlushTask *pFlushTask =
        MUTaskManager::getInstance()->createTask<LogFlushTask>(NULL);

    rt = pFlushTask->start();

    if (-1 == rt) {
        ERROR_LOG("Initialize operation log module failed. "
                  "Program will exit.");
        exit(1);
    }

    INFO_LOG("Intialize oepration log module.");
}

void
RunControl::initCSHeartbeat()
{
    int rt = 0;

    // CS heartbeat
    CSHeartbeatTask *pCSHeartbeatTask =
        MUTaskManager::getInstance()->createTask<CSHeartbeatTask>(NULL);
    rt = pCSHeartbeatTask->start();

    if (-1 == rt) {
        ERROR_LOG("Initialize heartbeat with CS failed. Program will exit.");
        exit(1);
    }

    INFO_LOG("Initialize heartbeat with CS.");
}

void
RunControl::initCSListen()
{
    int rt = 0;

    Epoll *pEpoll = NULL;
    pEpoll = MURegister::getInstance()->getEpoll();

    // CS listen
    MUTCPListenAgent<CSTask> *pCSListenAgent =
        new MUTCPListenAgent<CSTask>(pEpoll);

    SocketAddress addr;
    addr.setAddress(
        MUConfiguration::getInstance()->m_CSPMListenIP.c_str(),
        MUConfiguration::getInstance()->m_CSPMListenPort
    );

    rt = pCSListenAgent->init(addr);

    if (-1 == rt) {
        ERROR_LOG("Initialize CS listen socket failed. Program will exit.");

        exit(1);
    }

    INFO_LOG("Initialize CS listen socket.");
}

void
RunControl::initRecycleTask()
{
    int rt = 0;

    RecycleTask *pRecycleTask =
        MUTaskManager::getInstance()->createTask<RecycleTask>(NULL);
    rt = pRecycleTask->start();

    if (-1 == rt) {
        ERROR_LOG("Initialize recycle task failed. Program will exit.");
        exit(1);
    }

    INFO_LOG("Initialize recycle task for task GC.");
}

void
RunControl::startSystem()
{
    int rt = 0;

    // init task
    InitTask *pInitTask =
        MUTaskManager::getInstance()->createTask<InitTask>(NULL);
    rt = pInitTask->start();

    if (-1 == rt) {
        ERROR_LOG("Start system failed. Program will exit.");
        exit(1);
    }

    INFO_LOG("Now try to start the system.");
}

void
RunControl::initChannel()
{
	ChannelManager::getInstance()->init();

	//create Channel
	for(int i = 0; i < MUConfiguration::getInstance()->m_ChannelNum; i++){
	
		int ChannelID = i;
		std::string RootPath = MUConfiguration::getInstance()->m_ChannelVec[i];
		ChannelManager::getInstance()->createChannel(i, RootPath);
	}

	if(MUConfiguration::getInstance()->m_ChannelNum 
		!= ChannelManager::getInstance()->ChannelSize()){

		ERROR_LOG("initChannel() error. Start system failed. Program will exit.");
	}

	//create mapping strategy
	ChannelManager::getInstance()->createStrategy(SimpleHash);
	//just test
	Channel* pChannel = ChannelManager::getInstance()->Mapping(10);

	INFO_LOG("Initialize ChannelManager, %d Channel OK. test User10 Mapped to Channel*=0x%x OK.", 
							ChannelManager::getInstance()->ChannelSize(), pChannel);
}



void
RunControl::initFilesystemRoot()
{
    int rt = 0;
    struct stat st;

    std::string root = MUConfiguration::getInstance()->m_FileSystemRoot;

    rt = ::stat(root.c_str(), &st);

    if (0 == rt && S_ISDIR(st.st_mode)) {
        return ;
    }

    rt = ::mkdir(root.c_str(), S_IRWXU);

    if (-1 == rt) {
        DEBUG_LOG("mkdir() error, %s", strerror(errno));
        ERROR_LOG("Initialize filesystem root failed. Program will exit.");
        exit(1);
    }
}
void
RunControl::initMigrationListen()
{
    int rt = 0;

    Epoll *pEpoll = NULL;
    pEpoll = MURegister::getInstance()->getEpoll();

    // Migration listen
    MUTCPListenAgent<MigrationServerTask> *pMigrationListenAgent =
        new MUTCPListenAgent<MigrationServerTask>(pEpoll);

    SocketAddress addr;
    addr.setAddress(
        MUConfiguration::getInstance()->m_MigrationIP.c_str(),
        MUConfiguration::getInstance()->m_MigrationPort
    );

    rt = pMigrationListenAgent->init(addr);

    if (-1 == rt) {
        ERROR_LOG("Initialize migration listen socket failed. "
                  "Program will exit.");

        exit(1);
    }

    INFO_LOG("Initialize migration listen socket.");
}

void
RunControl::initCheckIdleUserTask()
{
    if (!(MUConfiguration::getInstance()->m_UserSerializable)) {
        // no lock for user
        return ;
    }

    int rt = 0;

    CheckIdleUserTask *pCheckIdleUserTask =
        MUTaskManager::getInstance()->createTask<CheckIdleUserTask>(NULL);
    rt = pCheckIdleUserTask->start();

    if (-1 == rt) {
        ERROR_LOG("Initialize idle users check task failed. Program will exit.");
        exit(1);
    }

    INFO_LOG("Initialize task for idle user checking.");
}



