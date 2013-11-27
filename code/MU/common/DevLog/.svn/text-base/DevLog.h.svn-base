#ifndef _DEVLOG_H_
#define _DEVLOG_H_

#define LOGPATH "./"
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include "FunctionTrace.h"
#define SEPARATOR_LINE "\n"
enum LOG_LEVEL { LEVEL_NONE, LEVEL_LOW, LEVEL_MIDDLE, LEVEL_HIGH, LEVEL_ALL};
const LOG_LEVEL LOG_LEVEL_ARR[]=
{
    LEVEL_NONE,
    LEVEL_LOW,
    LEVEL_MIDDLE,
    LEVEL_HIGH,
    LEVEL_ALL
};
enum LOG_TYPE
{
    LINFO=0,//一般信息，主要是各种xml，内容较长；蓝色打印
    LDEBUG, //调试信息，如某个变量的值；白色打印
    LFUNC, //函数进入退出；不打印
    LEVENT, //业务事件；绿色打印
    LWARNING, //警告；黄色打印
    LERROR, //一般错误；红色打印
    LFATAL//致命错误；红色打印
};

const std::string LEVEL_STR[]=
{
    "INFO",
    "DEBUG",
    "FUNC",
    "EVENT",
    "WARNING",
    "ERROR",
    "FATAL"
};
const int LEVEL_STR_MAX_LEN = 8;
enum LOG_OUT_TYPE {OUT_SCR=0, OUT_FILE, OUT_BOTH};

class DevLog
{
public:
    DevLog();
    DevLog(const char* logName);
    ~DevLog();

    int init();

    int setLogLevel(LOG_LEVEL level);
    int setLogPath(const std::string &path);//should be called before init()

    void writeLog(LOG_TYPE level, const std::string& message);
    void writeLog(LOG_TYPE level, LOG_OUT_TYPE type, const std::string& message);

    int setLineSeparator(const std::string &line);
    void insertSeparatorLine(int n=1);
private:
    void writeFile(const std::string &message);
    void printLog(LOG_TYPE level, const std::string& message);
    int getLogFileName(std::string &fname);
    int getCurrentTime(std::string &curtime);
    int checkDir(const char* dir);
int checkNewFile();
int getCurrentDate(std::string &date);
    //int getProcessId();
    //int getThreadId();

    int m_Fd;
    int m_ErrorCount;
    std::string m_LogPath;
    std::string m_FileNamePrefix;
    LOG_LEVEL m_LogLevel;
    std::string m_LineSeparator;
    std::string m_CurrentDate;
};

inline
std::string intToStr(int a)
{
    std::stringstream ss;
    ss << a;
    return ss.str();
}
#define DEV_LOG(event, outtype, msg) g_pDevLog->writeLog(event, outtype, (msg))
#define DEV_LOG_FUNC() FunctionTrace _temp(g_pDevLog, __FUNCTION__, __FILE__)
#define DEV_LOG_FUNC_MSG(msg) FunctionTrace _temp(g_pDevLog, __FUNCTION__, __FILE__, (msg))
#define DEV_LOG_ERROR(msg) g_pDevLog->writeLog(LERROR, OUT_BOTH, std::string(msg) + " [in " + std::string(__FILE__) + " on line " + std::string(intToStr(__LINE__)) + "]")

#endif
